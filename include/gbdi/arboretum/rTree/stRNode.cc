/**
* @file
*
* This file implements the RTree nodes.
*/
// Copyright (c) 2002 GBDI-ICMC-USP

//------------------------------------------------------------------------------
// class stRNode
//------------------------------------------------------------------------------
stRNode * stRNode::CreateNode(stPage * page){
   stRNode::stRNodeHeader * header;

   header = (stRNodeHeader *)(page->GetData());
   switch (header->Type){
      case INDEX:
         // Create an index page
         return new stRIndexNode(page, false);
      case LEAF:
         // Create a leaf page
         return new stRLeafNode(page, false);
      default:
         return NULL;
   }//end switch
}//end stRNode::CreateNode()

//------------------------------------------------------------------------------
// class stRIndexNode
//------------------------------------------------------------------------------
stRIndexNode::stRIndexNode(stPage * page, bool create): stRNode(page){

   // Attention to this manouver! It is the brain of this
   // implementation.
   Entries = (stRIndexEntry *)(page->GetData() + sizeof(stRNodeHeader));

   // Initialize page
   if (create){
      #ifdef __stDEBUG__
      Page->Clear();
      #endif //__stDEBUG__
      this->Header->Type = INDEX;
      this->Header->Occupation = 0;
   }//end if
}//end stRIndexNode::stRIndexNode()

//------------------------------------------------------------------------------
int stRIndexNode::AddEntry(stSize size, const stByte * object){
   stSize entrySize;

   // Does it fit ?
   entrySize = size + sizeof(stRIndexEntry);
   if (entrySize > this->GetFree()){
      // No, it doesn't.
      return -1;
   }//end if

   // Ok. I can put it. Lets put it in the last position.

   // Adding the object. Take care with these pointers or you will destroy the
   // node. The idea is to put the object of an entry in the reverse order
   // in the data array.
   if (Header->Occupation == 0){
      Entries[Header->Occupation].Offset = Page->GetPageSize() - size;
   }else{
      Entries[Header->Occupation].Offset = Entries[Header->Occupation - 1].Offset - size;
   }//end if
   memcpy((void *)(Page->GetData() + Entries[Header->Occupation].Offset), (void *)object, size);
   // Update # of entries
   Header->Occupation++; // One more!

   return Header->Occupation - 1;
}//end stRIndexNode::AddEntry()

//------------------------------------------------------------------------------
const stByte * stRIndexNode::GetObject(stCount idx){

   #ifdef __stDEBUG__
   if (idx >= GetNumberOfEntries()){
      throw invalid_argument("idx value is out of range.");
   }//end if
   #endif //__stDEBUG__

   return Page->GetData() + Entries[idx].Offset;
}//end stRIndexNode::GetObject()

//------------------------------------------------------------------------------
stSize stRIndexNode::GetObjectSize(stCount idx){
   #ifdef __stDEBUG__
   if (idx >= GetNumberOfEntries()){
      throw invalid_argument("idx value is out of range.");
   }//end if
   #endif //__stDEBUG__

   if (idx == 0){
      // First object
      return Page->GetPageSize() - Entries[0].Offset;
   }else{
      // Any other
      return Entries[idx - 1].Offset - Entries[idx].Offset;
   }//end if
}//end stRIndexNode::GetObjectSize()

//------------------------------------------------------------------------------
void stRIndexNode::RemoveEntry(stCount idx){
   stSize rObjSize;
   stCount i, lastID;

   // Programmer's note: This procedure is simple but tricky! See the
   // RIndexNode structure documentation for more details.

   #ifdef __stDEBUG__
   if (idx >= GetNumberOfEntries()){
      // Oops! This idx doesn't exists.
      throw range_error("idx is out of range.");
   }//end if
   #endif //__stDEBUG__

   // Let's remove
   lastID = Header->Occupation - 1; // The idx of the last object. This
                                    // value will be very useful.
   // Do I need to move something ?
   if (idx != lastID){
      // Yes, I do.
      rObjSize = GetObjectSize(idx);    // Save the removed object size

      // Let's move objects first. We will use memmove() from stdlib because
      // it handles the overlap between src and dst. Remember that src is the
      // offset of the last object and the dst is the offset of the last
      // object plus removed object size.
      memmove(Page->GetData() + Entries[lastID].Offset + rObjSize,
              Page->GetData() + Entries[lastID].Offset,
              Entries[idx].Offset - Entries[lastID].Offset);

      // Let's move entries...
      for (i = idx; i < lastID; i++){
         // Copy all fields with memcpy (it's faster than field copy).
         memcpy(Entries + i, Entries + i + 1, sizeof(stRIndexEntry));

         // Update offset by adding the removed object size to it. It will
         // reflect the previous move operation.
         Entries[i].Offset += rObjSize;
      }//end for
   }//end if

   // Update counter...
   Header->Occupation--;
}//end stRIndexNode::RemoveEntry()

//------------------------------------------------------------------------------
stSize stRIndexNode::GetFree(){
   stSize usedsize;

   // Fixed size
   usedsize = sizeof(stRNodeHeader);

   // Entries
   if (GetNumberOfEntries() > 0){
      usedsize +=
         // Total size of entries
         (sizeof(stRIndexEntry) * GetNumberOfEntries()) +
         // Total object size
         (Page->GetPageSize() - Entries[GetNumberOfEntries() - 1].Offset);
   }//end if

   return Page->GetPageSize() - usedsize;
}//end stRIndexNode::GetFree()
//------------------------------------------------------------------------------
stCount stRIndexNode::GetTotalObjectCount(){
   stSize count;
   stCount i;

   count = 0;
   for (i = 0; i < GetNumberOfEntries(); i++){
      count += GetIndexEntry(i).NEntries;
   }//end for

   return count;
}//end stRIndexNode::GetTotalObjectCount

//------------------------------------------------------------------------------
// class stRLeafNode
//------------------------------------------------------------------------------
stRLeafNode::stRLeafNode(stPage * page, bool create):
      stRNode(page){

   // Attention to this manouver! It is the brain of this
   // implementation.
   Entries = (stRLeafEntry*)(page->GetData() + sizeof(stRNodeHeader));

   // Initialize page
   if (create){
      #ifdef __stDEBUG__
      Page->Clear();
      #endif //__stDEBUG__
      this->Header->Type = LEAF;
      this->Header->Occupation = 0;
   }//end if
}//end stRLeafNode::stRLeafNode()

//------------------------------------------------------------------------------
int stRLeafNode::AddEntry(stSize size, const stByte * object){
   stSize entrySize;

   #ifdef __stDEBUG__
   if (size == 0){
      throw invalid_argument("The object size is 0.");
   }//end if
   #endif //__stDEBUG__

   // Does it fit ?
   entrySize = size + sizeof(stRLeafEntry);
   if (entrySize > this->GetFree()){
      // No, it doesn't.
      return -1;
   }//end if

   // Ok. I can put it. Lets put it in the last position.

   // Adding the object. Take care with these pointers or you will destroy the
   // node. The idea is to put the object of an entry in the reverse order
   // in the data array.
   if (Header->Occupation == 0){
      Entries[Header->Occupation].Offset = Page->GetPageSize() - size;
   }else{
      Entries[Header->Occupation].Offset = Entries[Header->Occupation - 1].Offset - size;
   }//end if
   memcpy((void *)(Page->GetData() + Entries[Header->Occupation].Offset),
          (void *)object, size);

   // Update # of entries
   Header->Occupation++; // One more!

   return Header->Occupation - 1;
}//end stRLeafNode::AddEntry()
//------------------------------------------------------------------------------
const stByte * stRLeafNode::GetObject(stCount idx){

   #ifdef __stDEBUG__
   if (idx >= GetNumberOfEntries()){
      throw invalid_argument("idx value is out of range.");
   }//end if
   #endif //__stDEBUG__

   return Page->GetData() + Entries[idx].Offset;
}//end stRLeafNode::GetObject()

//------------------------------------------------------------------------------
stSize stRLeafNode::GetObjectSize(stCount idx){
   #ifdef __stDEBUG__
   if (idx >= GetNumberOfEntries()){
      throw invalid_argument("idx value is out of range.");
   }//end if
   #endif //__stDEBUG__

   if (idx == 0){
      // First object
      return Page->GetPageSize() - Entries[0].Offset;
   }else{
      // Any other
      return Entries[idx - 1].Offset - Entries[idx].Offset;
   }//end if
}//end stRLeafIndexNode::GetObjectSize()

//------------------------------------------------------------------------------
void stRLeafNode::RemoveEntry(stCount idx){
   stCount lastID;
   stCount i;
   stSize rObjSize;

   // Programmer's note: This procedure is simple but tricky! See the
   // RIndexNode structure documentation for more details.

   #ifdef __stDEBUG__
   if (idx >= GetNumberOfEntries()){
      // Oops! This idx doesn't exists.
      throw range_error("idx value is out of range.");
   }//end if
   #endif //__stDEBUG__

   // Let's remove
   lastID = Header->Occupation - 1; // The idx of the last object. This
                                    // value will be very useful.
   // Do I need to move something ?
   if (idx != lastID){
      // Yes, I do.
      rObjSize = GetObjectSize(idx);    // Save the removed object size

      // Let's move objects first. We will use memmove() from stdlib because
      // it handles the overlap between src and dst. Remember that src is the
      // offset of the last object and the dst is the offset of the last
      // object plus removed object size.
      memmove(Page->GetData() + Entries[lastID].Offset + rObjSize,
              Page->GetData() + Entries[lastID].Offset,
              Entries[idx].Offset - Entries[lastID].Offset);

      // Let's move entries...
      for (i = idx; i < lastID; i++){
         // Copy all fields with memcpy (it's faster than field copy).
         memcpy(Entries + i, Entries + i + 1, sizeof(stRLeafEntry));

         // Update offset by adding the removed object size to it. It will
         // reflect the previous move operation.
         Entries[i].Offset += rObjSize;
      }//end for
   }//end if

   // Update counter...
   Header->Occupation--;
}//end stRLeafNode::RemoveEntry
//------------------------------------------------------------------------------
stSize stRLeafNode::GetFree(){
   stSize usedSize;

   // Fixed size
   usedSize = sizeof(stRNodeHeader);

   // Entries
   if (GetNumberOfEntries() > 0){

      usedSize +=
         // Total size of entries
         (sizeof(stRLeafEntry) * GetNumberOfEntries()) +
         // Total object size
         (Page->GetPageSize() - Entries[GetNumberOfEntries() - 1].Offset);
   }//end if

   return Page->GetPageSize() - usedSize;
}//end stRLeafNode::GetFree()
