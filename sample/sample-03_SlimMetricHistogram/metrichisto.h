#ifndef METRICHISTOH
#define METRICHISTOH

#include <arboretum/stTypes.h>
#include <arboretum/stObject.h>
#include <arboretum/stException.h>
#include <arboretum/stUserLayerUtil.h>
#include <math.h>

#include <assert.h>
#include <vector>

//------------------------------------------------------------------------------
// tMetricHistogramBin Class
//==============================================================================
/**
* This class implements the basic unit of the a Metric Histogram. It has two
* components, the Gray value and the value itself. It also implements the
* operators =, ==, < and >.
*
* @author Fabio Jun Takada Chino
* @author Josiane M. Bueno
* @version 1.0.2001.11.29
* @ingroup metrichisto
*/
class tMetricHistogramBin{
   public:
      /**
      * Creates a new instance of this class.
      */
      tMetricHistogramBin(){
         Gray = 0; // X axis
         Value = 0; // Y axis
      }//end tMetricHistogramBin

      /**
      * Set operator.
      */
      tMetricHistogramBin & operator = (tMetricHistogramBin & src){

         this->Gray = src.Gray;
         this->Value = src.Value;
         return *this;
      }//end operator =

      /**
      * @returns the gray value.
      */
      int GetGray(){
         return Gray;
      }//end GetGray

      /**
      * @returns the value.
      */
      double GetValue(){
         return Value;
      }//end GetValue

      /**
      * @returns the gray value in X.
      */
      int GetX(){
         return Gray;
      }//end GetX

      /**
      * @returns the value in Y.
      */
      double GetY(){
         return Value;
      }//end GetY

      /**
      * Sets the pair value.
      * @param gray
      * @param value
      */
      void Set(int gray, double value){
         Gray = gray;
         Value = value;
      }//end Set

      /**
      * Sets the gray value.
      * @param gray
      */
      void SetGray(int gray){ this->Gray = gray; }

      /**
      * Sets the value.
      * @param v (new value)
      */
      void SetValue(double v){ this->Value = v; }
   protected:

      /**
      * The gray value.
      */
      int Gray;

      /**
      * The value of the bin.
      */
      double Value;
};//end tMetricHistogramBin

class tMetricHistogram{
   public:
      /**
      * Creates a new empty metric histogram.
      */
      tMetricHistogram(){
         Serialized = NULL;
         Size = 0;
         Bins = NULL;
         maxBucketLen = 0;
      }//end tMetricHistogram

      /**
      * Creates a new metric histogram from a given histogram.
      *
      * @param src The source histogram.
      * @param tanThres
      * @param zeroThres
      * @return a new metric histogram from a given histogram.
      */
      //tMetricHistogram(tHistogram & src, double tanThres = 0.001, double zeroThres = 0.002, int maxBucketLen = 30); // deletado

      /**
      * Creates a new metric histogram from a given initialization data.
      *
      * @param size Number of Bins.
      * @param bins The bins.
      * @return a new metric histogram from a given initialization data
      */
      tMetricHistogram(int size, tMetricHistogramBin * bins);

      /**
      * Creates a new metric histogram from a Serialized data.
      *
      * @param SerializedData The Data serialized by Serialize() function.
      * @warning In order to keep the backward compatibility, this method does
      * not support the tag value even when it is present.
      * @return a new metric histogram.
      */
      tMetricHistogram(BYTE * SerializedData);

      /**
      * Creates a new metric histogram by coping another metric
      * histogram.
      *
      * @param mhistogram Another metric histogram.
      * @returns a new metric histogram by coping another metric
      * histogram.
      */
      tMetricHistogram(tMetricHistogram & mhistogram);

      /**
      * Creates a new instance of this class. All entries
      * will be load from a stream.
      * <p>The data stream must follow the same format used by
      * the method Dump().
      *
      * @param src The stream that contains the metric histogram.
      * @see Dump()
      * @returns a new instance of this class (a metric histogram).
      */
      //tMetricHistogram(tDataInputStream * src); // deletado

      /**
      * Disposes this instance and releases all associated resources.
      */
      ~tMetricHistogram(){
         if (Bins != NULL) {
            delete[] Bins;
         }//end if
         if (Serialized != NULL){
            delete [] Serialized;
         }//end if
      }//end ~tMetricHistogram

      /**
      * Returns the number of entries in this métric histogram. Since metric
      * histograms have variable number of entries, use this method to
      * @returns the number of entries in this metric histogram.
      */
      int GetSize(){
         return Size;
      }//end GetSize

      /**
      * Gets an entry (bin) of this metric histogram.
      *
      * @param idx The index of the bin.
      * @return The bin pair.
      * @warning Do not forgert to call Refresh() if you use this method to
      * modify the contents of this metric histogram.
      * @see Refresh()
      */
      tMetricHistogramBin & Get(int idx){
         return Bins[idx];
      }//end Get

      /**
      * Dumps the contents of this metric histogram to a stream.
      * <P>It will write a 16 bit integer to tell how many bins will
      * be written. After this, it will write all bins in sequence. Each
      * bin will have a 16 bits integer and a 64 bit floating point.
      *
      * @param out The output stream.
      */
      // void Dump(tDataOutputStream * out); // deletado

      /**
      * Returns the serialized version of this object. This method is required
      * by <i>stObject</i> interface.
      *
      * Serialize this Metric Histogram.
      *
      * <p><code>
      * +-------------------------------------------+--------------+<br>
      * | NBuckets |  bucket[0] | ... | bucket[n-1] |      tag     |<br>
      * |    int   | int|double | ... | int|double  | unsigned int |<br>
      * +-------------------------------------------+--------------+<br>
      * </code>
      * @returns the serialized version of this object.
      */
      const BYTE * Serialize();

      /**
      * Rebuilds a serialized object.
      *
      * @param data The serialized object.
      * @warning In order to keep the backward compatibility, this method does
      * not support the tag value even when it is present.
      */
      void Unserialize(const BYTE * data);

      // The following methods are required by the stObject interface.
		/**
      * @copydoc tMetricHistogram::Unserialize()
      *
      * @param SData The serialized data.
      * @param datasize The size of the serialized object in bytes.
      */
      void Unserialize(const stByte * SData, stSize dataSize);

      /**
      * @returns the serialized size.
      */
      DWORD GetSerializedSize(){
         return (sizeof(int) + ((sizeof(int) + sizeof(double)) * GetSize()) +
               sizeof(tag));
      }//end GetSerializedSize

      /**
      * Creates a perfect clone of this object. This method is required by
      * stObject interface.
      *
      * @return A new instance of tMetricHistogram wich is a perfect clone
      * of the original instance.
      */
      tMetricHistogram * Clone(){
         return new tMetricHistogram(*this);
      }//end Clone

      /**
      * Checks to see if this object is equal to other. This method is required
      * by stObject interface.
      *
      * @param obj Another instance of tMetricHistogram.
      * @return True if they are equal or false otherwise.
      */
      bool IsEqual(tMetricHistogram * obj);

      /**
      * Return the tag associated with this metric histogram. This value can be
      * used to associate this metric histogram to a data base entry.
      *
      * @return the tag associated with this metric histogram.
      */
      unsigned long GetTag(){ return tag; }

      /**
      * Sets the tag associated with this metric histogram. This value can be
      * used to associate this metric histogram to a data base entry.
      *
      * @param tag The tag value.
      */
      void SetTag(unsigned long tag){ this->tag = tag;}

      /**
      * Refreshes all internal buffers.
      */
      void Refresh();
   private:
      /**
      * Number of Bins in the metric histogram.
      */
      int Size;

      /**
      * The array of Bins of the metric histogram.
      */
      tMetricHistogramBin * Bins;

      /**
      * Zero threshold. Used by Signal.
      */
      double zeroThres;

      /**
      * Tangent threshold. Used by AcceptTan.
      */
      double tanThres;

      /**
      * Maximum size of a bucket.
      */
      int maxBucketLen;

      /**
      * This array holds the serialized version of this histogram.
      */
      stByte * Serialized;

      /**
      * Tag value. It may be used to associate this metric histogram to a
      * data base entry.
      */
      unsigned long tag;

      /**
      * Returns the signal of a given value. Any value between -zeroThres and
      * zeroThres is computed as zero.
      *
      * param value The value to be verified.
      * return 0 for zero, 1 for positive and -1 for negative.
      */
      int GetSignal(double value){

         if (fabsl(value) < zeroThres){
            return 0;
         }else if (value < 0){
            return -1;
         }else{
            return 1;
         }//end if
      }//end GetSignal

      /**
      * Tests to see if we will accept a given tangent. The tangent
      * will be accepted if the difference between angles exceeds
      * the tanThres. The angles are mesuared in radians.
      *
      * param l
      * param r
      */
      bool AcceptTan(double l, double r){
         double a = fabsl(atan(l) - atan(r));

         if (a > M_PI){
            a = 2 * M_PI - a ;
         }//end if

         return (a < tanThres);
         //return (fabsl(l - r) > tanThres);
      }//end AcceptTan

      /**
      * Metric histogram generator version 1.
      *
      * warning This method will be deprecated soon.
      */
      // void ProcessHistogramOld(tHistogram & src); // deletado

      /**
      * Creates a metric histogram from a given histogram.
      * This implementation has been changed to avoid work with no variation
      * of brightness. This is not the original aproach of processing histogram.
      */
      // void ProcessHistogram(tHistogram & src); // deletado

};//end tMetricHistogram

class tMetricHistogramEvaluator : public stMetricEvaluatorStatistics{
   public:

      /**
      * Returns the distance between 2 metricHistogram. This method is
      * required by stMetricEvaluator interface in stMetricTree.
      *
      * @param one Histogram 1.
      * @param other Histogram 2.
      * @return The distance between 2 metricHistogram.
      */
      stDistance GetDistance(tMetricHistogram * one, tMetricHistogram * other){
         // Update Statistics.
         UpdateDistanceCount();
         // Call MetricDistance.
         stDistance d = GetMetricDistance(one, other);
         return d;
      }//end GetDistance

      stDistance GetDistance2(tMetricHistogram * one, tMetricHistogram * other){
         stDistance d = GetDistance(one, other);
         return d * d;
      }//end GetDistance2

      /**
      * Compares two histograms and return the DM distance between them.
      *
      * @param one Histogram 1.
      * @param other Histogram 2.
      * @return The metric distance between them.
      */
      double GetMetricDistance(tMetricHistogram * one, tMetricHistogram * other);

      /**
      *
      * @param one Histogram 1.
      * @param other Histogram 2.
      * @return The Warp Distance.
      */
      double GetWarpDistance(tMetricHistogram * one, tMetricHistogram * other);

};//end tMetricHistogramEvaluator


//----------------------------------------------------------------------------
// Class template dlGenericMatrix
//----------------------------------------------------------------------------
/**
* This class template implements a generic nxm matrix.
*
* @author Fabio Jun Takada Chino
* @version 1.0
* @ingroup util
*/
template < class Type > class dlGenericMatrix{
   public:
      /**
      * Creates a new Matrix with 0 cols and 0 rows. Use SetSize() to change the
      * size of this matrix.
      */
      dlGenericMatrix(){
         Rows = 0;
         Cols = 0;
         Data = NULL;
      }//end stGenericMatrix

      /**
      * Creates a new matrix with a given size.
      *
      * @param col Columns.
      * @param row Rows.
      */
      dlGenericMatrix(int cols, int rows){
         Data = NULL;
         SetSize(cols, rows);
      }//end dlGenericMatrix

      /**
      * Disposes this matrix.
      */
      ~dlGenericMatrix(){

         if (Data != NULL){
            delete [] Data;
            delete [] PRows;
         }//end if
      }//end ~dlGenericMatrix

      /**
      * Sets the size of this matrix. All stored data will be lost!
      *
      * @param col Columns.
      * @param row Rows.
      */
      void SetSize(int cols, int rows);

      /**
      * Returns the number of rows.
      *
      * @return The number of rows.
      */
      int GetRows(){
         return Rows;
      }//end GetRows

      /**
      * Returns the number of columns.
      *
      * @return Returns the number of Columns.
      */
      int GetCols(){
         return Cols;
      }//end GetCols

      /**
      * Return a position of this matrix.
      *
      * @param col Column.
      * @param row Row.
      * @return Position of this matrix
      */
      Type & Get(int row, int col){
         return (*this)[row][col];
      }//end Get

      /**
      * This operator returns a row of this matrix. You may
      * use this operator as matrix[row][col].
      *
      * @param row Row.
      * @return Position of the row of this matrix
      */
      Type * operator [] (int row){
         return PRows[row];
      }//end operator []
   private:
      /**
      * Number of rows.
      */
      int Rows;

      /**
      * Number of columns.
      */
      int Cols;

      /**
      * Data vector.
      */
      Type * Data;

      /**
      * Speed up pointers.
      */
      Type ** PRows;
};//end dlGenericMatrix

//----------------------------------------------------------------------------
// Class template dlHugeGenericMatrix
//----------------------------------------------------------------------------
/**
* This class template implements is a variant of dlGenericMatrix which is able
* to store a very large matrix.
*
* @author Fabio Jun Takada Chino
* @version 1.0
* @ingroup util
*/
template < class Type > class dlHugeGenericMatrix{
   public:
      /**
      * Creates a new Matrix with 0 cols and 0 rows. Use SetSize() to change the
      * size of this matrix.
      */
      dlHugeGenericMatrix(){
         Rows = 0;
         Cols = 0;
         PRows = NULL;
      }//end dlHugeGenericMatrix

      /**
      * Creates a new matrix with a given size.
      *
      * @param col Columns.
      * @param row Rows.
      */
      dlHugeGenericMatrix(int cols, int rows){
         PRows = NULL;
         SetSize(cols, rows);
      }//end dlHugeGenericMatrix

      /**
      * Disposes this matrix.
      */
      virtual ~dlHugeGenericMatrix();

      /**
      * Sets the size of this matrix. All stored data will be lost!
      *
      * @param col Columns.
      * @param row Rows.
      */
      void SetSize(int cols, int rows);

      /**
      * Returns the number of rows.
      *
      * @return The number of Rows
      */
      int GetRows(){
         return Rows;
      }//end GetRows

      /**
      * Returns the number of Columns.
      *
      * @return The number of Columns
      */
      int GetCols(){
         return Cols;
      }//end GetCols

      /**
      * Return a position of this matrix.
      *
      * @param col Column.
      * @param row Row.
      */
      Type & Get(int row, int col){
         return (*this)[row][col];
      }//end Get

      /**
      * This operator returns a row of this matrix. You may
      * use this operator as matrix[row][col].
      *
      * @param row Row.
      */
      Type * operator [] (int row){
         return PRows[row];
      }//end operator []
   private:
      /**
      * Number of rows.
      */
      int Rows;

      /**
      * Number of columns.
      */
      int Cols;

      /**
      * Data pointers.
      */
      Type ** PRows;

      /**
      * Disposes all rows.
      */
      void DisposeRows();
};//end dlHugeGenericMatrix



/*
//------------------------------------------------------------------------------
// Classe tMetricHistogram
//==============================================================================
tMetricHistogram::tMetricHistogram(tHistogram & src, double tanThres,
            double zeroThres, int maxBucketLen){

   // Setup values.
   this->tanThres = tanThres;
   this->zeroThres = zeroThres;
   this->maxBucketLen = maxBucketLen;
   this->Serialized = NULL;
   this->tag = 0;

   // Process histogram to extract the metric histogram.
   ProcessHistogram(src);
}//end tMetricHistogram::tMetricHistogram
*/

//------------------------------------------------------------------------------
tMetricHistogram::tMetricHistogram(int size, tMetricHistogramBin * bins){

   Size = size;
   Bins = new tMetricHistogramBin[size];
   copy(bins, bins + size, Bins);
   this->tag = 0;
   
   // Serialized data
   Serialized = NULL;
}//end tMetricHistogram::tMetricHistogram

//------------------------------------------------------------------------------
tMetricHistogram::tMetricHistogram(BYTE * SerializedData){

   DWORD *tmp;
   Bins = NULL;
   this->tag = 0;
   this->Size = 0;
   this->Unserialize(SerializedData);
}//end tMetricHistogram::tMetricHistogram

//------------------------------------------------------------------------------
tMetricHistogram::tMetricHistogram(tMetricHistogram & mhistogram){

   Size = mhistogram.Size;
   Bins = new tMetricHistogramBin[Size];
   copy(mhistogram.Bins, mhistogram.Bins + Size, Bins);
   // Serialized data
   Serialized = NULL;
}//end tMetricHistogram::tMetricHistogram

/*
//------------------------------------------------------------------------------
tMetricHistogram::tMetricHistogram(tDataInputStream * src){
   int w;
   int n;
   int x;
   double y;
   tArch oldarch = src->GetArch();

   // Set encoding to little endian
   src->SetArch(LittleEndian);

   // Get Number of entries
   n = src->ReadInt16();

   // Allocate resources
   Size = n;
   Bins = new tMetricHistogramBin[Size];

   // Read and set bins
   for (w = 0; w < n; w++){
      x = src->ReadInt16();
      y = src->ReadFloat64();
      Bins[w].Set(x, y);
   }//end for

   src->SetArch(oldarch);
   // Serialized data
   Serialized = NULL;
}//end tMetricHistogram::tMetricHistogram
*/

/*
//------------------------------------------------------------------------------
void tMetricHistogram::Dump(tDataOutputStream * out){
   int w;
   tArch oldarch = out->GetArch();

   // Set encoding to little endian
   out->SetArch(LittleEndian);

   // Write the number of entries
   out->WriteInt16(Size);

   // Write entries
   for (w = 0; w < Size; w++){
      out->WriteInt16(Bins[w].GetX());
      out->WriteFloat64(Bins[w].GetY());
   }//end for

   out->SetArch(oldarch);
}//end tMetricHistogram::Dump
*/

//------------------------------------------------------------------------------
bool tMetricHistogram::IsEqual(tMetricHistogram * obj){
   // If the size of objects is diferent.
   if (this->GetSize() != obj->GetSize())
      return false;

   // For each bin compare the its contents.
   for (int idx=0; (idx<this->GetSize()) && (idx<obj->GetSize()); idx++){
      if ((this->Get(idx).GetX() != obj->Get(idx).GetX()) &&
          (this->Get(idx).GetY() != obj->Get(idx).GetY())){
          // If the content is diferent, then return false.
         return false;
      }//end if
   }//end for
   // return true if is equal.
   return true;
}//end IsEqual

//------------------------------------------------------------------------------
const BYTE * tMetricHistogram::Serialize(){
   BYTE *tmp;
   int w;

   // Attention: This is a modified version that supports tags. This
   // modification will not affect previous versions since the tag is the last
   // value and will be ignored by the old unserialization procedure.
   //
   // Note: I do love pointers! (Just kidding!)
   //                                                                     FJTC

   // Is there a serialized version ?
   if (Serialized == NULL){
      // We must create it.
      Serialized = new BYTE[GetSerializedSize()];

      //Write Buckets Count
      tmp = Serialized;
      *((int *)tmp) = GetSize();
      tmp += sizeof(int);

      for(w = 0; w < Size; w++){
        *((int *)tmp) = this->Bins[w].GetGray();
        tmp += sizeof(int);
        *((double *)tmp) = this->Bins[w].GetValue();
        tmp += sizeof(double);
      }//end for

      // Append the tag value at the end of file.
      *((unsigned long *)tmp) = this->tag;
   }//end if

   return Serialized;
}//end tMetricHistogram::Serialize()

//------------------------------------------------------------------------------
void tMetricHistogram::Unserialize(const BYTE * SData){
   int w;
   int oldSize;
   stByte * tmp;

   // Save the old size
   oldSize = Size;
   
   // Read the new size
   tmp = (stByte *)SData;
   Size = *((int*) tmp);
   tmp += sizeof(int);

   // Will reallocate things ?
   if (Size != oldSize){
      if (Bins != NULL){
         delete [] Bins;
      }//end if
      Bins = new tMetricHistogramBin[Size];
      
      if (Serialized != NULL){
         delete [] Serialized;
      }//end if
      Serialized = new stByte[GetSerializedSize()];
   }//end if

   for(w = 0; w < Size; w++){
      this->Bins[w].SetGray(*((int *)tmp));
      tmp += sizeof(int);
      this->Bins[w].SetValue(*((double *)tmp));
      tmp += sizeof(double);
   }//end for

   // It has no tag, so, ignore it!
   this->tag = 0;

   // Put all but the tag.
   memcpy(Serialized, SData, GetSerializedSize() - sizeof(tag));

   // Put the tag.
   *((unsigned int *)(Serialized + GetSerializedSize() - sizeof(tag))) =
         this->tag;
}//end tMetricHistogram::Unserialize

//------------------------------------------------------------------------------
void tMetricHistogram::Unserialize(const stByte * SData, stSize dataSize){
   int w;
   int oldSize;
   stByte * tmp;

   // Save the old size
   oldSize = Size;
   
   // Read the new size
   tmp = (stByte *)SData;
   Size = *((int*) tmp);
   tmp += sizeof(int);

   // Will reallocate things ?
   if (Size != oldSize){
      if (Bins != NULL){
         delete [] Bins;
      }//end if
      Bins = new tMetricHistogramBin[Size];
      
      if (Serialized != NULL){
         delete [] Serialized;
      }//end if
      Serialized = new stByte[GetSerializedSize()];
   }//end if

   for(w = 0; w < Size; w++){
      this->Bins[w].SetGray(*((int *)tmp));
      tmp += sizeof(int);
      this->Bins[w].SetValue(*((double *)tmp));
      tmp += sizeof(double);
   }//end for

   // Has the tag ?
   if (tmp != SData + dataSize){
      // Yes, it has a tag!
      this->tag = *((unsigned long *)tmp);
   }else{
      // No, it doesn't.
      this->tag = 0;
   }//end if

   // Put all but the tag.
   memcpy(Serialized, SData, GetSerializedSize() - sizeof(tag));

   // Put the tag.
   *((unsigned int *)(Serialized + GetSerializedSize() - sizeof(tag))) =
         this->tag;
}//end tMetricHistogram::Unserialize

/*
//------------------------------------------------------------------------------
void tMetricHistogram::ProcessHistogramOld(tHistogram & src){
   tMetricHistogramBin mhisto[256]; // Temp bin vector
   int n = 0;        // Number of bins in mhisto
   int w;            // Counter
   double ll;        // Left limit of h'(x)
   double rl;        // Right limit of h'(x)
   int lsig;         // Left signal
   bool accept;      // Accept bin

   // Add First entry
   mhisto[n].Set(0, src.GetData(0));
   n++;

   // Process entries
   for (w = 1; w < 254; w++){
      // Intilialize loop
      accept = false;

      // Left and right limits
      ll = src.GetData(w) - src.GetData(w - 1);
      rl = src.GetData(w + 1) - src.GetData(w);

      // Get signal
      lsig = GetSignal(ll);

      // Lets evaluate the point
      if (lsig != GetSignal(rl)){
         // Inversion of direction - Local min or max
         accept = true;
      }else if (lsig != 0) {
         // Same direction. Test angent limit
         accept = AcceptTan(ll, rl);
      }//end if

      if (accept){
         // Add bin
         mhisto[n].Set(w, src.GetData(w));
         n++;
      }else{
         if (GetSignal(mhisto[n - 1].GetY() - src.GetData(w)) != 0){
            if (w - mhisto[n - 1].GetX() > maxBucketLen){
               // Add bin because bucket len is too large
               mhisto[n].Set(w, src.GetData(w));
               n++;
            }//end if
         }//end if
      }//end if
   }//end for

   // Create the final vector.
   Bins = new tMetricHistogramBin[n + 1];
   Size = n + 1;
   copy(mhisto, mhisto + n, Bins);

   // Add last point
   Bins[n].Set(255, src.GetData(255));
}//end tMetricHistogram::ProcessHistogramOld
*/

/*
//------------------------------------------------------------------------------
void tMetricHistogram::ProcessHistogram(tHistogram & src){
   tMetricHistogramBin mhisto[256]; // Temp bin vector
   int n = 0;        // Number of bins in mhisto
   int w;            // Counter
   int b=0;          // Brightness value
   double ll;        // Left limit of h'(x)
   double rl;        // Right limit of h'(x)
   double th = 5E-5; // Threshold number to eliminate little variations
   int lsig;         // Left signal
   bool accept;      // Accept bin

   // Add First entry if it is bigger than the th value
   while (src.GetData(b) < th) b++;

   mhisto[n].Set(0, src.GetData(b));
   n++;

   // Process entries
   for (w = b+1 ; w < 254; w++){
      // w < 254 to ignore the last bin of normalized histogram
      accept = false;
      if (src.GetData(w) < th){
         b++;
      }else{
         // Left and right limits
         ll = src.GetData(w) - src.GetData(w - 1);
         rl = src.GetData(w + 1) - src.GetData(w);

         // Get signal
         lsig = GetSignal(ll);

         // Lets evaluate the point
         if (lsig != GetSignal(rl)){
            // Inversion of direction - Local min or max
            accept = true;
         }else if (lsig != 0) {
            // Same direction. Test angent limit
            accept = AcceptTan(ll, rl);
         }//end if

         if (accept){
            // Add bin
            mhisto[n].Set(w-b, src.GetData(w));
            n++;
         }else{
            if (GetSignal(mhisto[n - 1].GetY() - src.GetData(w)) != 0){
               if ((w-b) - mhisto[n - 1].GetX() > maxBucketLen){
                  // Add bin because bucket len is too large
                  mhisto[n].Set(w-b, src.GetData(w));
                  n++;
               }//end if
            }//end if
         }//end if
      }//end if
   }//end for

   // Create the final vector.
   Bins = new tMetricHistogramBin[n];// + 1];
   Size = n;// + 1;
   copy(mhisto, mhisto + n, Bins);

   // Add last point WHY??????
//   Bins[n].Set(255, src.GetData(255));
}//end tMetricHistogram::ProcessHistogram
*/

//------------------------------------------------------------------------------
void tMetricHistogram::Refresh(){

   if (this->Serialized != NULL){
      delete [] this->Serialized;
      this->Serialized = NULL;
   }//end if
}//end tMetricHistogram::Refresh

//------------------------------------------------------------------------------
// tMetricHistogramEvaluator Class
//==============================================================================

double tMetricHistogramEvaluator::GetMetricDistance(tMetricHistogram * one,
tMetricHistogram * other){
   double d=0.0;
   int    a=0, i1=1, io=1, base;
   double a1, ao, y1, yo, w;
   a1 = one->Get(0).GetValue();
   ao = other->Get(0).GetValue();

//   cout << "\n" << one->GetSize() << " " << other->GetSize();

   //enquanto existir buckets...
   while (i1 < one->GetSize() && io < other->GetSize()) {

      if (one->Get(i1).GetGray() < other->Get(io).GetGray() ) {
         base = one->Get(i1).GetGray() - a;
         yo = ao + base*(other->Get(io).GetValue() - ao)/(double)(other->Get(io).GetGray()-a);
         y1 = one->Get(i1++).GetValue();
      }else{
         base = other->Get(io).GetGray() - a;
         y1 = a1 + base*(one->Get(i1).GetValue() - a1)/(double)(one->Get(i1).GetGray() - a);
         yo = other->Get(io).GetValue();
         if (one->Get(i1).GetGray() == other->Get(io++).GetGray())
            i1++;
      }//end if
      if ( ((ao<a1) && (y1<yo)) || ((ao>a1) && (y1>yo)) ) { // lines cross
         w = (ao-a1)*base/(y1-a1-yo+ao);
         d += fabs(w*(ao-a1) + (base-w)*(y1-yo))/2.;
      }else{
         d += base*fabs(a1+y1-ao-yo);
      }//end if
      a1 = y1;
      ao = yo;
      a += base;
   }//end while
   return d/2.;

} //end tMetricHistogramEvaluator::GetMetricDistance

//------------------------------------------------------------------------------
double tMetricHistogramEvaluator::GetWarpDistance(tMetricHistogram * one,
        tMetricHistogram * other)
{
   int  i, j, lin, col, w;
   float distfinal;
   dlGenericMatrix<float> dist(256, 256), distacum(256,256);

//------vetq recebe os valores de one e vetc recebe os valores de other --------
    lin = one->GetSize();
    col = other->GetSize();
    float vetq[256], vetc[256];
    for (i=0; i<lin; i++)
      vetq[i] = one->Get(i).GetValue();
    for (j=0; j<col; j++)
      vetc[j] = other->Get(j).GetValue();

//------carga da matriz de distâncias ------------------------------------------
    for(i=0; i<lin; i++){
      for(j=0; j<col; j++){
         dist[i][j] = (fabs((vetq[i]-vetc[j])) * fabs(vetq[i]-vetc[j]));
      }//end for
    }//end for
//-------matriz de distâncias acumuladas------------------------------
    float d1, d2, d3;
    for(i=0; i<lin; i++){
      for(j=0; j<col; j++){
         if(i == 0){
            if(j == 0)
               distacum[i][j] = dist[i][j];
            else
               distacum[i][j] = dist[i][j] + distacum[i][j-1];
         }else{
            if(j == 0)
               distacum[i][j] = dist[i][j] + distacum[i-1][j];
            else{
               d1 = distacum[i-1][j-1];
               d2 = distacum[i-1][j];
               d3 = distacum[i][j-1];
               if (d1<=d2 && d1<=d3) distacum[i][j] = dist[i][j] + d1;
               else if (d2<=d3) distacum[i][j] = dist[i][j] + d2;
               else distacum[i][j] = dist[i][j] + d3;
            }//end else j!=0
         }//end else (i!=0)
      }//end for
    }//end for

//------quantidade de elementos do caminho (w) ---------------------------------
    w = 1;
    i = lin-1;
    j = col-1;
    do
    {   if(i == 0)
        {   if(j != 0)  j--;           
        }
        else
        {   if(j == 0)  i--;            
            else
            {   d1 = distacum[i-1][j-1];
                d2 = distacum[i-1][j];
                d3 = distacum[i][j-1];
                if (d1<=d2 && d1<=d3) { i--; j--; }
                else if (d2<=d3) i--;
                else j--;
            }
        }
        w++;
        //printf(" w = %d", w);

    }while(i>0 || j>0);

//------distância final --------------------------------------------------------
    distfinal = distacum[lin-1][col-1]/w;
    return distfinal;
} //end tMetricHistogramEvaluator::GetWarpDistance
//------------------------------------------------------------------------------


//----------------------------------------------------------------------------
// Class template dlGenericMatrix
//----------------------------------------------------------------------------
template < class Type >
void dlGenericMatrix<Type>::SetSize(int cols, int rows){
   int w;

 	if (Data != NULL){
      delete [] Data;
    	delete [] PRows;
   }//end if

   Rows = rows;
   Cols = cols;
   Data = new Type[cols * rows];
   PRows = new Type * [rows];
   PRows[0] = Data;
   for (w = 1; w < rows; w++){
      PRows[w] = PRows[w - 1] + cols;
   }//end for
}//end dlGenericMatrix<Type>::SetSize

//----------------------------------------------------------------------------
// Class template dlGenericMatrix
//----------------------------------------------------------------------------
template < class Type >
dlHugeGenericMatrix<Type>::~dlHugeGenericMatrix(){

   DisposeRows();
}//end stHugeGenericMatrix<Type>::~stHugeGenericMatrix

//----------------------------------------------------------------------------
template < class Type >
void dlHugeGenericMatrix<Type>::SetSize(int cols, int rows){
   int w;

   if ((cols != Cols) || (rows != Rows)){
      // Kill previows matrix
      DisposeRows();

      Rows = rows;
      Cols = cols;
      PRows = new Type * [rows];
      for (w = 0; w < rows; w++){
         PRows[w] = new Type[cols];
      }//end for      
   //}else{
      // Do nothing. He he he...
   }//end if
}//end dlGenericMatrix<Type>::SetSize

//----------------------------------------------------------------------------
template < class Type >
void dlHugeGenericMatrix<Type>::DisposeRows(){
   Type ** i;

 	if (PRows != NULL){
      for (i = PRows; i < PRows + Rows; i++){
         delete [] *i;
      }//end for
      delete [] PRows;
   }//end if
}//end dlGenericMatrix<Type>::DisposeRows

#endif
