//
// Copyright (C) 2017 Reinhard Prix
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with with program; see the file COPYING. If not, write to the
// Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
// MA 02111-1307 USA
//

///
/// \file
/// \ingroup lalapps_pulsar_Weave
///

#include "Statistics.h"

#include <lal/LALString.h>

///
/// struct defining the global 'statistics map' that contains all the defining properties
/// of the supported statistics
///
typedef struct tagWeaveStatisticMap {
  WeaveStatisticType val;		///< bitflag value for this statistic
  const char *const name;		///< internal name of this statistics
  WeaveStatisticType dependencies;      ///< set of *direct* input dependencies of this statistic
  const char *const help;               ///< help string explaining this statistic
} WeaveStatisticMap;

///
/// sets of toplists, extra statistics and dependencies handled by this code
///
#define ENTRY_NONE              WEAVE_STATISTIC_NONE,           "none", "         ", 0, \
    "No statistic selected"

#define ENTRY_COH2F             WEAVE_STATISTIC_COH2F,          "coh2F", "        ", 0, \
    "Per-segment multi-detector coherent 2F statistic"

#define ENTRY_COH2F_DET         WEAVE_STATISTIC_COH2F_DET,      "coh2F_det", "    ", 0, \
    "Per-segment per-detector coherent 2F statistic"

#define ENTRY_MAX2F             WEAVE_STATISTIC_MAX2F,          "max2F", "        ", WEAVE_STATISTIC_COH2F,     \
    "Maximum over segments multi-detector coherent 2F statistic"

#define ENTRY_MAX2F_DET         WEAVE_STATISTIC_MAX2F_DET,      "max2F_det", "    ", WEAVE_STATISTIC_COH2F_DET, \
    "Maximum over segments per-detector coherent 2F statistic"

#define ENTRY_SUM2F             WEAVE_STATISTIC_SUM2F,          "sum2F", "        ", WEAVE_STATISTIC_COH2F,     \
    "Sum over segments of multi-detector coherent 2F statistic"

#define ENTRY_SUM2F_DET         WEAVE_STATISTIC_SUM2F_DET,      "sum2F_det", "    ", WEAVE_STATISTIC_COH2F_DET, \
    "Sum over segments of single-detector coherent 2F statistic"

#define ENTRY_MEAN2F            WEAVE_STATISTIC_MEAN2F,         "mean2F", "       ", WEAVE_STATISTIC_SUM2F,     \
    "Average over segments of multi-detector coherent 2F statistic"

#define ENTRY_MEAN2F_DET        WEAVE_STATISTIC_MEAN2F_DET,     "mean2F_det", "   ", WEAVE_STATISTIC_SUM2F_DET, \
    "Average over segments of single-detector coherent 2F statistic"

#define ENTRY_BSGL              WEAVE_STATISTIC_BSGL,           "B_S/GL", "       ", WEAVE_STATISTIC_SUM2F|WEAVE_STATISTIC_SUM2F_DET, \
    "Bayes factor 'Signal' vs 'Gaussian noise' or 'Line'"

#define ENTRY_BSGLtL            WEAVE_STATISTIC_BSGLtL,         "B_S/GLtL", "     ", WEAVE_STATISTIC_SUM2F|WEAVE_STATISTIC_SUM2F_DET|WEAVE_STATISTIC_MAX2F_DET, \
    "Bayes factor 'Signal' vs 'Gaussian noise' or 'Line' or 'transient Line'."

#define ENTRY_BtSGLtL           WEAVE_STATISTIC_BtSGLtL,        "B_tS/GLtL", "    ", WEAVE_STATISTIC_MAX2F|WEAVE_STATISTIC_SUM2F_DET|WEAVE_STATISTIC_MAX2F_DET, \
    "Bayes factor 'transient Signal' vs 'Gaussian noise' or 'Line' or 'transient Line'."

#define ENTRY_NCOUNT            WEAVE_STATISTIC_NCOUNT,         "ncount", "       ", WEAVE_STATISTIC_COH2F,     \
    "Multi-detector 'Hough' number count of 'threshold crossings' heavyside(2F - 2Fth) over segments"

#define ENTRY_NCOUNT_DET        WEAVE_STATISTIC_NCOUNT_DET,     "ncount_det", "   ", WEAVE_STATISTIC_COH2F_DET, \
    "Per-detector 'Hough' number count of 'threshold crossings' heavyside(2F - 2Fth) over segments"

#define ENTRY_2_MAP(X) ENTRY_2_MAP_X(X)
#define ENTRY_2_MAP_X(v,n,s,d,h)  { .val = v, .name = n, .dependencies = d, .help = h }

#define ENTRY_2_CHOICES(X) ENTRY_2_CHOICES_X(X)
#define ENTRY_2_CHOICES_X(v,n,s,d,h) { .val = v, .name = n }

#define ENTRY_2_HELPSTR(X) ENTRY_2_HELPSTR_X(X)
#define ENTRY_2_HELPSTR_X(v,n,s,d,h) " - " n s ": " h ".\n"

///
/// (Sparse) array of descriptor structs for all statistics supported by Weave
///
const WeaveStatisticMap statistic_map[] = {
  ENTRY_2_MAP(ENTRY_COH2F),
  ENTRY_2_MAP(ENTRY_COH2F_DET),
  ENTRY_2_MAP(ENTRY_MAX2F),
  ENTRY_2_MAP(ENTRY_MAX2F_DET),
  ENTRY_2_MAP(ENTRY_SUM2F),
  ENTRY_2_MAP(ENTRY_SUM2F_DET),
  ENTRY_2_MAP(ENTRY_MEAN2F),
  ENTRY_2_MAP(ENTRY_MEAN2F_DET),
  ENTRY_2_MAP(ENTRY_BSGL),
  ENTRY_2_MAP(ENTRY_BSGLtL),
  ENTRY_2_MAP(ENTRY_BtSGLtL),
  ENTRY_2_MAP(ENTRY_NCOUNT),
  ENTRY_2_MAP(ENTRY_NCOUNT_DET),
};

// total set of current supported statistics
#define SUPPORTED_STATISTICS (                                          \
    0                                                                   \
    | WEAVE_STATISTIC_COH2F                                             \
    | WEAVE_STATISTIC_COH2F_DET                                         \
    | WEAVE_STATISTIC_SUM2F                                             \
    | WEAVE_STATISTIC_SUM2F_DET                                         \
    | WEAVE_STATISTIC_MEAN2F                                            \
    | WEAVE_STATISTIC_MEAN2F_DET                                        \
    | WEAVE_STATISTIC_BSGL                                              \
    )
const UserChoices WeaveStatisticChoices = {
  ENTRY_2_CHOICES(ENTRY_NONE),
  ENTRY_2_CHOICES(ENTRY_COH2F),
  ENTRY_2_CHOICES(ENTRY_COH2F_DET),
  ENTRY_2_CHOICES(ENTRY_SUM2F),
  ENTRY_2_CHOICES(ENTRY_SUM2F_DET),
  ENTRY_2_CHOICES(ENTRY_MEAN2F),
  ENTRY_2_CHOICES(ENTRY_MEAN2F_DET),
  ENTRY_2_CHOICES(ENTRY_BSGL),
  { SUPPORTED_STATISTICS, "all" }
};
const char *const WeaveStatisticHelpString =
  ENTRY_2_HELPSTR(ENTRY_COH2F)
  ENTRY_2_HELPSTR(ENTRY_COH2F_DET)
  ENTRY_2_HELPSTR(ENTRY_SUM2F)
  ENTRY_2_HELPSTR(ENTRY_SUM2F_DET)
  ENTRY_2_HELPSTR(ENTRY_MEAN2F)
  ENTRY_2_HELPSTR(ENTRY_MEAN2F_DET)
  ENTRY_2_HELPSTR(ENTRY_BSGL)
  ;

// subset of statistics that are supported as toplist ranking statistics
#define SUPPORTED_TOPLISTS (                            \
    0                                                   \
    | WEAVE_STATISTIC_MEAN2F                            \
    | WEAVE_STATISTIC_SUM2F                             \
    | WEAVE_STATISTIC_BSGL                              \
    )
const UserChoices WeaveToplistChoices = {
  ENTRY_2_CHOICES(ENTRY_MEAN2F),
  ENTRY_2_CHOICES(ENTRY_SUM2F),
  ENTRY_2_CHOICES(ENTRY_BSGL),
  {SUPPORTED_TOPLISTS, "all" }
};
const char *const WeaveToplistHelpString =
  ENTRY_2_HELPSTR(ENTRY_MEAN2F)
  ENTRY_2_HELPSTR(ENTRY_SUM2F)
  ENTRY_2_HELPSTR(ENTRY_BSGL)
  ;

///
/// set all bits in 'deps' corresponding to *direct* dependencies of the set of input statistics 'stat'
///
int XLALWeaveStatisticsSetDirectDependencies(
  WeaveStatisticType *deps,
  const WeaveStatisticType stats
  )
{
  XLAL_CHECK ( (stats & ~SUPPORTED_STATISTICS) == 0, XLAL_EINVAL );

  WeaveStatisticType tmp = 0;
  for ( size_t i=0; i < XLAL_NUM_ELEM(statistic_map); ++i ) {
    if ( stats & statistic_map[i].val ) {
      tmp |= statistic_map[i].dependencies;
    }
  }

  ( *deps ) |= tmp;

  return XLAL_SUCCESS;

} // XLALWeaveStatisticsSetDependencies()


///
/// Fill StatisticsParams logic for given toplist and extra-output stats
///
int XLALWeaveStatisticsParamsSetDependencyMap(
  WeaveStatisticsParams *statistics_params,
  const WeaveStatisticType toplist_stats,
  const WeaveStatisticType extra_output_stats
  )
{
  XLAL_CHECK ( statistics_params != NULL, XLAL_EFAULT );
  XLAL_CHECK ( (toplist_stats & (~SUPPORTED_TOPLISTS)) == 0, XLAL_EINVAL );
  XLAL_CHECK ( (extra_output_stats & (~SUPPORTED_STATISTICS)) == 0, XLAL_EINVAL );

  WeaveStatisticType stats_to_output = (toplist_stats | extra_output_stats);

  // work out the total set of all statistics we need to compute by
  // expanding the statistics dependencies until converged [tree fully expanded]
  WeaveStatisticType stats_to_compute = stats_to_output;    // start value
  WeaveStatisticType mainloop_stats  = toplist_stats;      // start value
  WeaveStatisticType prev_stats_to_compute, prev_mainloop_stats;
  do {
    prev_stats_to_compute = stats_to_compute;
    prev_mainloop_stats  = mainloop_stats;

    XLALWeaveStatisticsSetDirectDependencies( &stats_to_compute, stats_to_compute );
    XLALWeaveStatisticsSetDirectDependencies( &mainloop_stats, mainloop_stats );

  } while ( (prev_stats_to_compute != stats_to_compute) && (prev_mainloop_stats != mainloop_stats) );

  // special handling of 'coh2F' and 'coh2F_det': these can *only* be computed as "main-loop" statistics!
  // as they are defined to refer to the 'fine grid with (typically) interpolation', while
  // non-interpolating "recalc" 2F-per-segments statistics will be named differently // FIXME: put chosen names
  if ( stats_to_compute & WEAVE_STATISTIC_COH2F ) {
    mainloop_stats |= WEAVE_STATISTIC_COH2F;
  }
  if ( stats_to_compute & WEAVE_STATISTIC_COH2F_DET ) {
    mainloop_stats |= WEAVE_STATISTIC_COH2F_DET;
  }

  WeaveStatisticType completionloop_stats = stats_to_compute & (~mainloop_stats);

  // figure out which mainloop statistics to keep outside of main loop: either
  // 1) because they have been requested for output, or
  // 2) they are a _direct_ completionloop dependency,
  // all other mainloop stats can be thrown away safely after the mainloop.
  WeaveStatisticType mainloop_stats_to_keep = 0;

  // 1) if requested for output:
  mainloop_stats_to_keep |= (mainloop_stats & stats_to_output);

  // 2) if *direct* completionloop dependencies:
  WeaveStatisticType completionloop_deps = 0;
  XLALWeaveStatisticsSetDirectDependencies ( &completionloop_deps, completionloop_stats );
  mainloop_stats_to_keep |= (mainloop_stats & completionloop_deps);

  // store the resulting statistics logic in the 'statistics_params' struct
  statistics_params -> toplist_statistics           = toplist_stats;
  statistics_params -> statistics_to_output         = stats_to_output;
  statistics_params -> statistics_to_compute        = stats_to_compute;
  statistics_params -> mainloop_statistics          = mainloop_stats;
  statistics_params -> mainloop_statistics_to_keep  = mainloop_stats_to_keep;
  statistics_params -> completionloop_statistics    = completionloop_stats;

  return XLAL_SUCCESS;

} // XLALWeaveStatisticsParamsSetDependencyMap()

///
/// Destroy a StatisticsParams struct
///
void XLALWeaveStatisticsParamsDestroy(
  WeaveStatisticsParams *statistics_params
  )
{
  if ( statistics_params == NULL ) {
    return;
  }

  XLALDestroyStringVector ( statistics_params -> detectors );
  XLALDestroyBSGLSetup ( statistics_params -> BSGL_setup );
  XLALFree ( statistics_params );

  return;

} // XLALStatisticsParamsDestroy()


// Local Variables:
// c-file-style: "linux"
// c-basic-offset: 2
// End:
