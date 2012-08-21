
/*!
\ingroup PkgBooleanSetOperations2Concepts
\cgalconcept

\refines ::Functor 

\hasModel ArrangementDirectionalXMonotoneTraits_2::CompareEndpointsXy_2 

*/

class ArrDirectionalTraits::CompareEndpointsXy_2 {
public:

/// \name Has Models 
/// @{

/*! 
accepts an input curve `xc` and compares its source and target 
points. It returns `SMALLER` if the curve is directed from 
lexicographically left to right, and `LARGER` if it is directed 
from lexicographically right to left. 
*/ 
Comparison_result operator()(ArrDirectionalTraits::X_monotone_curve_2 xc); 

/// @}

}; /* end ArrDirectionalTraits::CompareEndpointsXy_2 */

