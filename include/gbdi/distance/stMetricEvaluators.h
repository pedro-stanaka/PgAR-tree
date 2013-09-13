/**********************************************************************
* GBDI Arboretum - Copyright (c) 2002-2004 GBDI-ICMC-USP
*
*                           Homepage: http://gbdi.icmc.usp.br/arboretum
**********************************************************************/
/* ====================================================================
 * The GBDI-ICMC-USP Software License Version 1.0
 *
 * Copyright (c) 2004 Grupo de Bases de Dados e Imagens, Instituto de
 * Ci�ncias Matem�ticas e de Computa��o, University of S�o Paulo -
 * Brazil (the Databases and Image Group - Intitute of Matematical and 
 * Computer Sciences).  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:
 *       "This product includes software developed by Grupo de Bases
 *        de Dados e Imagens, Instituto de Ci�ncias Matem�ticas e de
 *        Computa��o, University of S�o Paulo - Brazil (the Databases 
 *        and Image Group - Intitute of Matematical and Computer 
 *        Sciences)"
 *
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names of the research group, institute, university, authors
 *    and collaborators must not be used to endorse or promote products
 *    derived from this software without prior written permission.
 *
 * 5. The names of products derived from this software may not contain
 *    the name of research group, institute or university, without prior
 *    written permission of the authors of this software.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OF THIS SOFTWARE OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * ====================================================================
 *                                            http://gbdi.icmc.usp.br/
 */
/**
* @file
*
* This file implements all basic metric evaluators templates.
*
* @version 1.0
* $Revision: 1.31 $
* $Date: 2009-01-10 18:34:52 $
*
* $Author: willian $
* @author Fabio Jun Takada Chino (chino@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
* @todo Documentation review and tests.
*/
// Copyright (c) 2002-2003 GBDI-ICMC-USP
#ifndef __STMETRICEVALUATORS_H
#define __STMETRICEVALUATORS_H

#include <gbdi/datatype/stTypes.h>
#include <gbdi/util/stUtil.h>
#include <gbdi/distance/stUserLayerUtil.h>
#include <gbdi/datatype/stObject.h>
#include <gbdi/datatype/stBasicObjects.h>
#include <gbdi/distance/stMetricEvaluator.h>

#include <stdlib.h>
#include <vector>


//==============================================================================
// Class template stMetricEvaluator
//------------------------------------------------------------------------------
/**
* This abstract class is the basic class for all stXXXXMetricEvaluator classes.
*
* @auth../../../../include/arboretum/stMetricEvaluators.h:215:   instantiated from ‘stDistance stEuclideanMetricEvaluator<DType>::GetDistance(stObject*, stObject*) [with DType = stArrayObject<double, int>]’or Fabio Jun Takada Chino (chino@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
* @ingroup user
*/
/*template <class DType>
class stMetricEvaluator: public stMetricEvaluatorStatistics{
   public:   
      stMetricEvaluator(){}
      
      stMetricEvaluator(const stCount d): stMetricEvaluatorStatistics(d){}
      
      stMetricEvaluator(const stMetricEvaluator& evaluator): stMetricEvaluatorStatistics(evaluator){}
   
      virtual ~stMetricEvaluator(){}
      
      
      // @copydoc stMetricEvaluator::GetDistance()
      virtual stDistance GetDistance(stObject * o1, stObject * o2) = 0;

      
      // @copydoc stMetricEvaluator::GetDistance2()
      virtual stDistance GetDistance2(stObject * o1, stObject * o2) = 0;
};//end stMetricEvaluator
*/
//==============================================================================
// Class template stLInfinityMetricEvaluator
//------------------------------------------------------------------------------
/**
* This class implements a generic metric evaluator that calculates the distance
* between objects using the L0 or LInfinity distance function.
*
* <P>The template parameter <b>DType</b> defines the object type return from the
* method  <b>DType * Get(n)</b>. 
*
* <P>Note that to be fully compatible with the metric trees, the desired object
* must meet the requirements of the stObject interface also.
*
* @author Adriano Arantes Paterlini (paterlini@icmc.usp.br)
* @warning Both objects must have the same number of entries.
* @todo Documentation.
* @todo Tests.
* @see stMetricEvaluator
* @see stEuclideanMetricEvaluator
* @see stManhatanMetricEvaluator
* @see stLxMetricEvaluator
* @ingroup user
*/
template <class DType>
class stLInfinityMetricEvaluator: public stMetricEvaluator, public stMetricEvaluatorStatistics {
   public:
      stLInfinityMetricEvaluator(){}
      
      stLInfinityMetricEvaluator(const stCount d){}
      
      stLInfinityMetricEvaluator(const stLInfinityMetricEvaluator& evaluator){}
   
      virtual ~stLInfinityMetricEvaluator(){}
   
      /**
      * @copydoc stMetricEvaluator::GetDistance()
      */
      virtual stDistance GetDistance(stObject * o1, stObject * o2);

      /**
      * @copydoc stMetricEvaluator::GetDistance2()
      */
      virtual stDistance GetDistance2(stObject * o1, stObject * o2){
         stDistance d;

         d = GetDistance(o1, o2);

         return d * d;
      }//end GetDistance2
};//end stLInfinityMetricEvaluator

//==============================================================================
// Class template stEuclideanMetricEvaluator
//------------------------------------------------------------------------------
/**
* This class implements a generic metric evaluator that calculates the distance
* between objects using the L2 or the Euclidean distance function.
*
* <P>The template parameter <b>DType</b> defines the object type used by
* this metric evaluator. It was designed to work with instances of stArray
* objects but any array object that defines the following methods may be used:
*  - <b>type * Get(n)</b>: Returns the element at position <i>n</i>.
*  - <b>int * GetSize()</b>: Returns the number of element int hte array.
*
* <P>Note that to be fully compatible with the metric trees, the desired object
* must meet the requirements of the stObject interface also.
*
* @author Fabio Jun Takada Chino (chino@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
* @warning Both objects must have the same number of entries.
* @todo Documentation.
* @todo Tests.
* @see stMetricEvaluator
* @see stLInfinityMetricEvaluator
* @see stManhatanMetricEvaluator
* @see stLxMetricEvaluator
* @ingroup user
*/
template <class DType>
class stEuclideanMetricEvaluator: public stMetricEvaluator, public stMetricEvaluatorStatistics {
   public:
      stEuclideanMetricEvaluator(){}
      
      stEuclideanMetricEvaluator(const stCount d){}
      
      stEuclideanMetricEvaluator(const stEuclideanMetricEvaluator& evaluator){}
   
      virtual ~stEuclideanMetricEvaluator(){}   
   
      /**
      * @copydoc stMetricEvaluator::GetDistance()
      */
      virtual stDistance GetDistance(stObject * o1, stObject * o2){

         return sqrt(GetDistance2(o1, o2));
      }//end GetDistance

      /**
      * @copydoc stMetricEvaluator::GetDistance2()
      */
      virtual stDistance GetDistance2(stObject * o1, stObject * o2);

};//end stEuclideanMetricEvaluator

//==============================================================================
// Class template stManhatanMetricEvaluator
//------------------------------------------------------------------------------
/**
* This class implements a generic metric evaluator that calculates the distance
* between objects using the L1 or the Manhatan distance function.
*
* <P>The template parameter <b>DType</b> defines the object type used by
* this metric evaluator. It was designed to work with instances of stArray
* objects but any array object that defines the following methods may be used:
*  - <b>type * Get(n)</b>: Returns the element at position <i>n</i>.
*  - <b>int * GetSize()</b>: Returns the number of element int hte array.
*
* <P>Note that to be fully compatible with the metric trees, the desired object
* must meet the requirements of the stObject interface also.
*
* @author Fabio Jun Takada Chino (chino@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
* @warning Both objects must have the same number of entries.
* @todo Documentation.
* @todo Tests.
* @see stMetricEvaluator
* @see stLInfinityMetricEvaluator
* @see stEuclideanMetricEvaluator
* @see stLxMetricEvaluator
* @ingroup user
*/
template <class DType>
class stManhatanMetricEvaluator: public stMetricEvaluator, public stMetricEvaluatorStatistics {
   public:
      stManhatanMetricEvaluator(){}
      
      stManhatanMetricEvaluator(const stCount d){}
      
      stManhatanMetricEvaluator(const stManhatanMetricEvaluator& evaluator){}
   
      virtual ~stManhatanMetricEvaluator(){}    
   
      /**
      * @copydoc stMetricEvaluator::GetDistance()
      */
      virtual stDistance GetDistance(stObject * o1, stObject * o2);

      /**
      * @copydoc stMetricEvaluator::GetDistance2()
      */
      virtual stDistance GetDistance2(stObject * o1, stObject * o2){
         stDistance d;

         d = GetDistance(o1, o2);

         return d * d;
      }//end GetDistance2
};//end stManhatanMetricEvaluator




//==============================================================================
// Class template stCanberraMetricEvaluator
//------------------------------------------------------------------------------
/**
* This class implements a generic metric evaluator that calculates the distance
* between objects using the Canberra distance function.
*
* <P>The template parameter <b>DType</b> defines the object type used by
* this metric evaluator. It was designed to work with instances of stArray
* objects but any array object that defines the following methods may be used:
*  - <b>type * Get(n)</b>: Returns the element at position <i>n</i>.
*  - <b>int * GetSize()</b>: Returns the number of element int hte array.
*
* <P>Note that to be fully compatible with the metric trees, the desired object
* must meet the requirements of the stObject interface also.
*
* @author Javier Montoya (jmontoya@lis.ic.unicamp.br)
* @warning Both objects must have the same number of entries.
* @todo Documentation.
* @todo Tests.
* @see stMetricEvaluator
* @ingroup user
*/

template <class DType>
class stCanberraMetricEvaluator: public stMetricEvaluator, public stMetricEvaluatorStatistics {
   public:
      stCanberraMetricEvaluator(){}
      
      stCanberraMetricEvaluator(const stCount d){}
      
      stCanberraMetricEvaluator(const stCanberraMetricEvaluator& evaluator){}
   
      virtual ~stCanberraMetricEvaluator(){}     
   
      /**
      * @copydoc stMetricEvaluator::GetDistance()
      */
      virtual stDistance GetDistance(stObject * o1, stObject * o2);

      /**
      * @copydoc stMetricEvaluator::GetDistance2()
      */
      virtual stDistance GetDistance2(stObject * o1, stObject * o2){
         stDistance d;

         d = GetDistance(o1, o2);

         return d * d;
      }//end GetDistance2
};//end stCanberraMetricEvaluator

//==============================================================================
// Class template stBrayCurtisMetricEvaluator
//------------------------------------------------------------------------------
/**
* This class implements a generic metric evaluator that calculates the distance
* between objects using the Bray Curtis distance function.
*
* <P>The template parameter <b>DType</b> defines the object type used by
* this metric evaluator. It was designed to work with instances of stArray
* objects but any array object that defines the following methods may be used:
*  - <b>type * Get(n)</b>: Returns the element at position <i>n</i>.
*  - <b>int * GetSize()</b>: Returns the number of element int hte array.
*
* <P>Note that to be fully compatible with the metric trees, the desired object
* must meet the requirements of the stObject interface also.
*
* @author Javier Montoya (jmontoya@lis.ic.unicamp.br)
* @warning Both objects must have the same number of entries.
* @todo Documentation.
* @todo Tests.
* @see stMetricEvaluator
* @ingroup user
*/

template <class DType>
class stBrayCurtisMetricEvaluator: public stMetricEvaluator, public stMetricEvaluatorStatistics {
   public:
      stBrayCurtisMetricEvaluator(){}
      
      stBrayCurtisMetricEvaluator(const stCount d){}
      
      stBrayCurtisMetricEvaluator(const stBrayCurtisMetricEvaluator& evaluator){}
   
      virtual ~stBrayCurtisMetricEvaluator(){}        
   
      /**
      * @copydoc stMetricEvaluator::GetDistance()
      */
      virtual stDistance GetDistance(stObject * o1, stObject * o2);

      /**
      * @copydoc stMetricEvaluator::GetDistance2()
      */
      virtual stDistance GetDistance2(stObject * o1, stObject * o2){
         stDistance d;

         d = GetDistance(o1, o2);

         return d * d;
      }//end GetDistance2
};//end stBrayCurtisMetricEvaluator

//==============================================================================
// Class template stSquaredChordMetricEvaluator
//------------------------------------------------------------------------------
/**
* This class implements a generic metric evaluator that calculates the distance
* between objects using the Bray Curtis distance function.
*
* <P>The template parameter <b>DType</b> defines the object type used by
* this metric evaluator. It was designed to work with instances of stArray
* objects but any array object that defines the following methods may be used:
*  - <b>type * Get(n)</b>: Returns the element at position <i>n</i>.
*  - <b>int * GetSize()</b>: Returns the number of element int hte array.
*
* <P>Note that to be fully compatible with the metric trees, the desired object
* must meet the requirements of the stObject interface also.
*
* @author Javier Montoya (jmontoya@lis.ic.unicamp.br)
* @warning Both objects must have the same number of entries.
* @todo Documentation.
* @todo Tests.
* @see stMetricEvaluator
* @ingroup user
*/

template <class DType>
class stSquaredChordMetricEvaluator: public stMetricEvaluator, public stMetricEvaluatorStatistics {
   public:
      stSquaredChordMetricEvaluator(){}
      
      stSquaredChordMetricEvaluator(const stCount d){}
      
      stSquaredChordMetricEvaluator(const stSquaredChordMetricEvaluator& evaluator){}
   
      virtual ~stSquaredChordMetricEvaluator(){}       
   
      /**
      * @copydoc stMetricEvaluator::GetDistance()
      */
      virtual stDistance GetDistance(stObject * o1, stObject * o2);

      /**
      * @copydoc stMetricEvaluator::GetDistance2()
      */
      virtual stDistance GetDistance2(stObject * o1, stObject * o2){
         stDistance d;

         d = GetDistance(o1, o2);

         return d * d;
      }//end GetDistance2
};//end stSquaredChordMetricEvaluator

//==============================================================================
// Class template stSquareChiSquaredMetricEvaluator
//------------------------------------------------------------------------------
/**
* This class implements a generic metric evaluator that calculates the distance
* between objects using the Bray Curtis distance function.
*
* <P>The template parameter <b>DType</b> defines the object type used by
* this metric evaluator. It was designed to work with instances of stArray
* objects but any array object that defines the following methods may be used:
*  - <b>type * Get(n)</b>: Returns the element at position <i>n</i>.
*  - <b>int * GetSize()</b>: Returns the number of element int hte array.
*
* <P>Note that to be fully compatible with the metric trees, the desired object
* must meet the requirements of the stObject interface also.
*
* @author Javier Montoya (jmontoya@lis.ic.unicamp.br)
* @warning Both objects must have the same number of entries.
* @todo Documentation.
* @todo Tests.
* @see stMetricEvaluator
* @ingroup user
*/

//template <class DType>
//class stSquareChiSquaredMetricEvaluator: public stMetricEvaluator, public stMetricEvaluatorStatistics {
//   public:
//      stSquareChiSquaredMetricEvaluator(){}
//
//      stSquareChiSquaredMetricEvaluator(const stCount d): stMetricEvaluator < DType >(d){}
//
//      stSquareChiSquaredMetricEvaluator(const stSquareChiSquaredMetricEvaluator& evaluator): stMetricEvaluator < DType >(evaluator){}
//
//      virtual ~stSquareChiSquaredMetricEvaluator(){}
//
//      /**
//      * @copydoc stMetricEvaluator::GetDistance()
//      */
//      virtual stDistance GetDistance(stObject * o1, stObject * o2);
//
//      /**
//      * @copydoc stMetricEvaluator::GetDistance2()
//      */
//      virtual stDistance GetDistance2(stObject * o1, stObject * o2){
//         stDistance d;
//
//         d = GetDistance(o1, o2);
//
//         return d * d;
//      }//end GetDistance2
//};//end stSquareChiSquaredMetricEvaluator

//==============================================================================
// Class template stLpMetricEvaluator
//------------------------------------------------------------------------------
/**
* This class implements a generic metric evaluator that calculates the distance
* between objects using an Lx distance function. The value of p may be
* any number greater than 0. In other words, this class implements L1,
* L2, L3, ..., Lp distance function.
*
* <P>The template parameter <b>DType</b> defines the object type used by
* this metric evaluator. It was designed to work with instances of stArray
* objects but any array object that defines the following methods may be used:
*  - <b>type * Get(n)</b>: Returns the element at position <i>n</i>.
*  - <b>int * GetSize()</b>: Returns the number of element int hte array.
*
* <P>Note that to be fully compatible with the metric trees, the desired object
* must meet the requirements of the stObject interface also.
*
* <P>The template parameter <b>P</b> is the order of L. For example, it it is
* set to 3, it will implement the L3 distance function. If you set it to 2, it
* will implements the L2 or the Euclidean distance function.
*
* <P>Important Note: For L1 or L2 it is possible to use this class template with
* the P set as 1 or 2 but it is better to use stManhatanMetricEvaluator or
* stEuclideanMetricEvaluator instead because they have better
* implementations.
*
* @author Fabio Jun Takada Chino (chino@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
* @warning Both objects must have the same number of entries.
* @todo Documentation.
* @todo Tests.
* @see stMetricEvaluator
* @see stLInfinityMetricEvaluator
* @see stEuclideanMetricEvaluator
* @see stManhatanMetricEvaluator
* @ingroup user
* @warning Certain values of p may break the triangular inequality.
*/
//template <class DType, int P = 3>
//class stLpMetricEvaluator: public stMetricEvaluator, public stMetricEvaluatorStatistics {
//   public:
//      /**
//      * @copydoc stMetricEvaluator::GetDistance()
//      */
//      virtual stDistance GetDistance(stObject * o1, stObject * o2);
//
//      /**
//      * @copydoc stMetricEvaluator::GetDistance2()
//      */
//      virtual stDistance GetDistance2(stObject * o1, stObject * o2){
//         stDistance d;
//
//         d = GetDistance(o1, o2);
//
//         return d * d;
//      }//end GetDistance2
//};//end stLpMetricEvaluator

//==============================================================================
// Class template stLp2MetricEvaluator
//------------------------------------------------------------------------------
/**
* This class is very similar to stLpMetricEvaluator except that it's
* constructor allows passing the P parameter at runtime.
*
* <P>This class implements a generic metric evaluator that calculates the distance
* between objects using an Lx distance function. The value of p may be
* any number greater than 0. In other words, this class implements L1,
* L2, L3, ..., Lp distance function.
*
* <P>The template parameter <b>DType</b> defines the object type used by
* this metric evaluator. It was designed to work with instances of stArray
* objects but any array object that defines the following methods may be used:
*  - <b>type * Get(n)</b>: Returns the element at position <i>n</i>.
*  - <b>int * GetSize()</b>: Returns the number of element int hte array.
*
* <P>Note that to be fully compatible with the metric trees, the desired object
* must meet the requirements of the stObject interface also.
*
* <P>The template parameter <b>P</b> is the order of L. For example, it it is
* set to 3, it will implement the L3 distance function. If you set it to 2, it
* will implements the L2 or the Euclidean distance function.
*
* <P>Important Note: For L1 or L2 it is possible to use this class template with
* the P set as 1 or 2 but it is better to use stManhatanMetricEvaluator or
* stEuclideanMetricEvaluator instead because they have better
* implementations.
*
* @author Humberto Razente (hlr@icmc.usp.br)
* @warning Both objects must have the same number of entries.
* @todo Documentation.
* @todo Tests.
* @see stMetricEvaluator
* @see stLInfinityMetricEvaluator
* @see stEuclideanMetricEvaluator
* @see stManhatanMetricEvaluator
* @ingroup user
* @warning Certain values of p may break the triangular inequality.
*/
//template <class DType>
//class stLp2MetricEvaluator: public stMetricEvaluator, public stMetricEvaluatorStatistics {
//
//   private:
//       int P;
//
//   public:
//      /**
//      * This constructor allows passing the P parameter at runtime.
//      *
//      * @param p Value of P.
//      */
//      stLp2MetricEvaluator(int p) {
//         if (p > 0){
//            P = p;
//         }else{
//            P = 3;
//         }//end if
//      }//end stLp2MetricEvaluator
//
//      /**
//      * @copydoc stMetricEvaluator::GetDistance()
//      */
//      virtual stDistance GetDistance(stObject * o1, stObject * o2);
//
//      /**
//      * @copydoc stMetricEvaluator::GetDistance2()
//      */
//      virtual stDistance GetDistance2(stObject * o1, stObject * o2){
//         stDistance d;
//
//         d = GetDistance(o1, o2);
//
//         return d * d;
//      }//end GetDistance2
//};//end stLp2MetricEvaluator

//==============================================================================
// Class template stLEditMetricEvaluator
//------------------------------------------------------------------------------
/**
* This class implements a generic metric evaluator that calculates the distance
* between strings using the LEdit distance function.
*
* <P>The template parameter <b>DType</b> defines the object type used by
* this metric evaluator. It was designed to work with instances of stArray
* objects but any array object that defines the following methods may be used:
*  - <b>type * Get(n)</b>: Returns the element at position <i>n</i>.
*  - <b>int * GetSize()</b>: Returns the number of element int hte array.
*
* <P>Note that to be fully compatible with the metric trees, the desired object
* must meet the requirements of the stObject interface also.
*
* <P>The template parameter DataType defines the type of each character. By
* default, it is set to char.
*
* <P>This implementation was based on the article "Levenshtein Distance, in
* Three Flavors" written by
* <a href="http://www.mgilleland.com/">Michael Gilleland</a>.
*
* @author Fabio Jun Takada Chino (chino@icmc.usp.br)
* @author Marcos Rodrigues Vieira (mrvieira@icmc.usp.br)
* @todo Documentation.
* @todo Tests. I don't know if this implementation is reflexive.
* @see stMetricEvaluator
* @ingroup user
*/
template <class DType = char>
class stLEditMetricEvaluator: public stMetricEvaluator, public stMetricEvaluatorStatistics {
   public:
      stLEditMetricEvaluator(){}
      
      stLEditMetricEvaluator(const stCount d) {}
      
      stLEditMetricEvaluator(const stLEditMetricEvaluator& evaluator){}
   
      virtual ~stLEditMetricEvaluator(){}      
   
      /**
      * @copydoc stMetricEvaluator::GetDistance()
      */
      virtual stDistance GetDistance(stObject * o1, stObject * o2){
         
         // Statistic support
         this->UpdateDistanceCount();
         return LEdit(*o2, *o1);
      }//end GetDistance

      /**
      * @copydoc stMetricEvaluator::GetDistance2()
      */
      virtual stDistance GetDistance2(stObject * o1, stObject * o2){
         stDistance d;

         d = GetDistance(o1, o2);

         return d * d;
      }//end GetDistance2
      
   private:
      /**
      * Type of the diff matrix.
      */
      typedef stGenericMatrix <DType> tMatrix;

      /**
      * The diff matrix.
      */
      tMatrix diff;      

      /**
      * Returns the minimum value between 3 values.
      *
      * @param a Value a.
      * @param b Value b.
      * @param c Value c.
      */
      DType Min(DType a, DType b, DType c){
         if (a < b){
            if (a < c){
               return a;
            }else{
               return c;
            }//end if
         }else{
            if (b < c){
               return b;
            }else{
               return c;
            }//end if
         }//end if
      }//end Min

      /**
      * Performs the LEdit.
      *
      * @param si String 1.
      * @param sj String 2.
      * @return The distance beween wShort and wLong.
      */
      stDistance LEdit(stObject & si, stObject & sj);
      
};//end stLEditMetricEvaluator


class SumProductMetric : public stMetricEvaluator, public stMetricEvaluatorStatistics {
public:

    typedef stBasicArrayObject <double> BasicArrayDoubleObject;
    typedef stMetricEvaluator myMetricEvaluator;

    SumProductMetric() {
        numEval = 0;

    }

    virtual ~SumProductMetric() {
        Clear();
    }

    void Clear(){
        for (int i = 0; i < numEval; i++) {
             delete Evaluator[i];
         }
         Evaluator.clear();
         numEval = 0;
    }

    void AddEval(myMetricEvaluator * e, stDistance w) {

        Evaluator.insert(Evaluator.end(), e);
        max.insert(max.end(), 0);
        weight.insert(weight.end(), w);
        numEval++;
    }

    stDistance GetPartialDistance(stObject * o1, stObject * o2, int i) {

        stDistance tmp;
        stDistance Sum = 0;
        int NumObjs = o1->GetSize();

        if (i < NumObjs) {
            //tmp = Evaluator[i]->GetDistance((BasicArrayDoubleObject *) o1->GetObj(i), (BasicArrayDoubleObject *) o2->GetObj(i));
            tmp = Evaluator[i]->GetDistance((stObject *)o1->Get(i), (stObject *)o2->Get(i));
            Sum += (weight[i] * tmp);
        }

        this->UpdateDistanceCount();

        return Sum;
    }


    /**
     * @copydoc stMetricEvaluator::GetDistance()
     */
    virtual stDistance GetDistance(stObject * o1, stObject * o2) {

        stDistance Sum = 0;
        stDistance tmp;
        int NumObjs = o1->GetSize();

        for (int i = 0; i < NumObjs && i < numEval; i++) {
            //cout << typeid(*(o1->GetObj(i))).name() << endl;
            tmp = Evaluator[i]->GetDistance((stObject *)o1->Get(i), (stObject *)o2->Get(i));
            if (weight[i] * tmp > max[i])  max[i] = weight[i] * tmp;
            Sum += (weight[i] * tmp);

           // Sum +=Evaluator[i]->GetDistance(o1->GetObj(i), o2->GetObj(i));
        }

        this->UpdateDistanceCount();

        return Sum;
    }

    /**
     * @copydoc stMetricEvaluator::GetDistance2()
     */
    virtual stDistance GetDistance2(stObject * o1, stObject * o2) {
        stDistance d;

        d = GetDistance(o1, o2);

        return d * d;
    }//end GetDistance2

    stDistance GetMax(int i){
        return max[i];
    }

private:

    int numEval;

    vector <stDistance> max;

    vector <stDistance> weight;

    vector <myMetricEvaluator *> Evaluator;

}; //end SumProductMetric

// Include implementation
#include "stMetricEvaluators.cc"

#endif //__STMETRICEVALUATORS_H
