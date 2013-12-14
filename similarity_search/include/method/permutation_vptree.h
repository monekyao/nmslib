/**
 * Non-metric Space Library
 *
 * Authors: Bilegsaikhan Naidan (https://github.com/bileg), Leonid Boytsov (http://boytsov.info).
 * With contributions from Lawrence Cayton (http://lcayton.com/).
 *
 * For the complete list of contributors and further details see:
 * https://github.com/searchivarius/NonMetricSpaceLib 
 * 
 * Copyright (c) 2010--2013
 *
 * This code is released under the
 * Apache License Version 2.0 http://www.apache.org/licenses/.
 *
 */

#ifndef _PERMUTATION_VPTREE_SAMPLE_H_
#define _PERMUTATION_VPTREE_SAMPLE_H_

#include <vector>

#include "index.h"
#include "space_lp.h"
#include "permutation_utils.h"
#include "vptree.h"
#include "params.h"
#include "searchoracle.h"

/* 
 * The current sampling method works worse than stretching of the triangle inequality
 * We don't remove this completely, b/c we hope to significantly improve sampling one day.
 */
//#define USE_VPTREE_SAMPLE

#define METH_PERMUTATION_VPTREE     "perm_vptree"

namespace similarity {

/*
 * The hybrid of the permutation index and of the vptree. 
 * It is experimental and is not especially efficient.
 *
 * A very similar idea was proposed by Figueroa and Fredriksson in
 * Speeding up permutation based indexing with indexing (2009)
 *
 * The difference from their work: we search permutations using 
 * APPROXIMATE near-neighbor search, while Figueroa and Fredriksson are 
 * using an exact one.
 *
 */
template <typename dist_t, PivotIdType (*CorrelDistFunc)(const PivotIdType*, const PivotIdType*, size_t)>
class PermutationVPTree : public Index<dist_t> {
 public:
  PermutationVPTree(const Space<dist_t>* space,
                   const ObjectVector& data,
                   const AnyParams& MethPars);

  ~PermutationVPTree();

  const std::string ToString() const;
  void Search(RangeQuery<dist_t>* query);
  void Search(KNNQuery<dist_t>* query);

 private:
  const Space<dist_t>*      space_;
  const ObjectVector&       data_;
  size_t                    db_scan_qty_;
  ObjectVector              pivots_;
  ObjectVector              PermData_;

#ifdef USE_VPTREE_SAMPLE
  VPTree<PivotIdType, SamplingOracle<PivotIdType>, SamplingOracleCreator<PivotIdType> >*   VPTreeIndex_;
#else
  VPTree<float, TriangIneq<float>, TriangIneqCreator<float> >*   VPTreeIndex_;
#endif
#ifdef USE_VPTREE_SAMPLE
  const CorrelVectorSpace<CorrelDistFunc>*                                         VPTreeSpace_;
#else
  const SpaceLp<float>*                                                            VPTreeSpace_;
#endif

  // disable copy and assign
  DISABLE_COPY_AND_ASSIGN(PermutationVPTree);
};

}  // namespace similarity

#endif     // _PERMUTATION_INDEX_H_

