/*********************** <lalVerbatim file="OverlapReductionFunctionTestCV">
Author: UTB Relativity Group; contact whelan@oates.utb.edu
$Id$
********************************* </lalVerbatim> */

/********************************************************** <lalLaTeX>
\subsection{Program \texttt{OverlapReductionFunctionTest.c}}
\label{stochastic:ss:OverlapReductionFunctionTest.c}

A program to test \texttt{LALOverlapReductionFunction()}.

\subsubsection*{Usage}

\begin{verbatim}
./OverlapReductionFunctionTest [options]
Options:
  -h             print usage message
  -q             quiet: run silently
  -v             verbose: print extra information
  -d level       set lalDebugLevel to level
  -s siteID1     calculate overlap red fcn for site siteID1
  -t siteID2       with site siteID2
  -f f0          set start frequency to f0
  -e deltaF      set frequency spacing to deltaF
  -n length      set number of points in frequency series to length
  -o filename    print overlap reduction function to file filename
\end{verbatim}

\subsubsection*{Description}


This program tests the function {\tt LALOverlapReductionFunction()\/}, which calculates
the overlap reduction (\ref{stochastic:e:gamma(f)}) for a pair of gravitational
wave detectors.

First, it tests that the correct error codes 
(\textit{cf.}\ Sec.~\ref{stochastic:s:StochasticCrossCorrelation.h})
are generated for the following error conditions (tests in
\textit{italics} are not performed if \verb+LAL_NDEBUG+ is set, as
the corresponding checks in the code are made using the ASSERT macro):
\begin{itemize}
\item \textit{null pointer to parameter structure}
\item \textit{zero length parameter}
\item \textit{negative frequency spacing}
\item \textit{zero frequency spacing}
\item \textit{null pointer to output series}
\item \textit{null pointer to data member of output series}
\item mismatch between length of output series and length parameter
\item \textit{null pointer to data member of data member of output series}
\item negative start frequency
\item non-symmetric response tensor
\end{itemize}

It then verifies that the correct frequency series are generated for
some simple test cases:
\begin{enumerate}
\item co\"{\i}ncident, co\"aligned interferometers: $\gamma(f)=1$
\item co\"aligned interferometers lying parallel to the $x$--$y$ plane
  separated only in $z$:
  $\gamma(f)=\frac{5}{2}\alpha^{-1}\sin\alpha(1-3\alpha^{-2}+3\alpha^{-5})
  +\frac{5}{2}\alpha^{-1}\cos\alpha(2-3\alpha^{-2})$, where
  $\alpha=2\pi f|\Delta\vec x|/c$
\item completely misaligned interferometers lying parallel to the
  $x$--$y$ plane separated only in $z$: $\gamma(f)=0$.
\end{enumerate}

If the \texttt{filename} argument is present, it also calculates a
spectrum based on user-specified data.
Figure~\ref{stochastic:f:LHOLLO}-\ref{stochastic:f:GEO600TAMA300} illustrates 
the output of the overlap reduction function with the corresponding command arguments of each detector.

% begin figures %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% \begin{figure}[htb!]
% \begin{center}
% \noindent\includegraphics[width=4in,angle=-90]{stochasticOverlapFigLLOLHO}
% \caption{\label{stochastic:f:LHOLLO}
% Overlap reduction function for the LHO--LLO pair.}
% \end{center}
% \end{figure}
%

\begin{figure}[htb!]
  \begin{center}
    \noindent
    \includegraphics[width=3.5in,angle=-90]{stochasticOverlapLHOLLO}
    \caption{\label{stochastic:f:LHOLLO}
      Overlap reduction function for the LHO--LLO pair.
      This was generated by the command \newline 
      \hbox{\tt
OverlapReductionFunctionTest -e 1 -n 10000 -s 0 -t 1 -o LHOLLOOverlap.dat
}}

  \end{center}
\end{figure}

\begin{figure}[htb!]
  \begin{center}
    \noindent
    \includegraphics[width=3.5in,angle=-90]{stochasticOverlapLHOVIRGO}
    \caption{\label{stochastic:f:LHOVIRGO}
      Overlap reduction function for the LHO--VIRGO pair. 
      This was generated by the command \newline 
      \hbox{\tt
OverlapReductionFunctionTest -e 1 -n 10000 -s 0 -t 2 -o LHOVIRGOOverlap.dat
}}
  \end{center}
\end{figure}

\begin{figure}[htb!]
  \begin{center}
    \noindent
    \includegraphics[width=3.5in,angle=-90]{stochasticOverlapLHOGEO600}
    \caption{\label{stochastic:f:LHOGEO600}
      Overlap reduction function for the LHO--GEO600 pair.
      This was generated by the command \newline 
      \hbox{\tt
OverlapReductionFunctionTest -e 1 -n 10000 -s 0 -t 3 -o LHOGEO600Overlap.dat
}}
  \end{center}
\end{figure}

\begin{figure}[htb!]
  \begin{center}
    \noindent
    \includegraphics[width=3.5in,angle=-90]{stochasticOverlapLHOTAMA300}
    \caption{\label{stochastic:f:LHOTAMA300}
      Overlap reduction function for the LHO--TAMA300 pair.
      This was generated by the command \newline 
      \hbox{\tt
OverlapReductionFunctionTest -e 1 -n 10000 -s 0 -t 4 -o LHOTAMA300Overlap.dat
}}
  \end{center}
\end{figure}

\begin{figure}[htb!]
  \begin{center}
    \noindent
    \includegraphics[width=3.5in,angle=-90]{stochasticOverlapLLOVIRGO}
    \caption{\label{stochastic:f:LLOVIRGO}
      Overlap reduction function for the LLO--VIRGO pair.
      This was generated by the command \newline 
      \hbox{\tt
OverlapReductionFunctionTest -e 1 -n 10000 -s 1 -t 2 -o LLOVIRGOOverlap.dat
}}
  \end{center}
\end{figure}

\begin{figure}[htb!]
  \begin{center}
    \noindent
    \includegraphics[width=3.5in,angle=-90]{stochasticOverlapLLOGEO600}
    \caption{\label{stochastic:f:LLOGEO600}
      Overlap reduction function for the LLO--GEO600 pair.
      This was generated by the command \newline 
      \hbox{\tt
OverlapReductionFunctionTest -e 1 -n 10000 -s 1 -t 3 -o LLOGEO600Overlap.dat
}}
  \end{center}
\end{figure}

\begin{figure}[htb!]
  \begin{center}
    \noindent
    \includegraphics[width=3.5in,angle=-90]{stochasticOverlapLLOTAMA300}
    \caption{\label{stochastic:f:LLOTAMA300}
      Overlap reduction function for the LLO--TAMA300 pair.
      This was generated by the command \newline 
      \hbox{\tt
OverlapReductionFunctionTest -e 1 -n 10000 -s 1 -t 4 -o LLOTAMA300Overlap.dat
}}
  \end{center}
\end{figure}

\begin{figure}[htb!]
  \begin{center}
    \noindent
    \includegraphics[width=3.5in,angle=-90]{stochasticOverlapVIRGOGEO600}
    \caption{\label{stochastic:f:VIRGOGEO600}
      Overlap reduction function for the VIRGO--GEO600 pair.
      This was generated by the command \newline 
      \hbox{\tt
OverlapReductionFunctionTest -e 1 -n 10000 -s 2 -t 3 -o VIRGOGEO600Overlap.dat
}}
  \end{center}
\end{figure}

\begin{figure}[htb!]
  \begin{center}
    \noindent
    \includegraphics[width=3.5in,angle=-90]{stochasticOverlapVIRGOTAMA300}
    \caption{\label{stochastic:f:VIRGOTAMA300}
      Overlap reduction function for the VIRGO--TAMA300 pair.
      This was generated by the command \newline
      \hbox{\tt
OverlapReductionFunctionTest -e 1 -n 10000 -s 2 -t 4 -o VIRGOTAMA300Overlap.dat
}}
  \end{center}
\end{figure}

\begin{figure}[htb!]
  \begin{center}
    \noindent
    \includegraphics[width=3.5in,angle=-90]{stochasticOverlapGEO600TAMA300}
    \caption{\label{stochastic:f:GEO600TAMA300}
      Overlap reduction function for the GEO600--TAMA300 pair.
      This was generated by the command \newline 
      \hbox{\tt
OverlapReductionFunctionTest -e 1 -n 10000 -s 3 -t 4 -o GEO600TAMA300Overlap.dat
}}
  \end{center}
\end{figure}

% end figures %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

\subsubsection*{Exit codes}
\input{OverlapReductionFunctionTestCE}

\subsubsection*{Uses}

\begin{verbatim}
lalDebugLevel
getopt()
LALSCreateVector()
LALOverlapReductionFunction()
LALSPrintFrequencySeries()
LALSDestroyVector()
LALCheckMemoryLeaks()
\end{verbatim}

******************************************************* </lalLaTeX> */

#include <lal/LALStdlib.h>

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <config.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif

#include <lal/StochasticCrossCorrelation.h>
#include <lal/AVFactories.h>
#include <lal/PrintFTSeries.h>
#include <lal/Units.h>

#include "CheckStatus.h"

NRCSID(OVERLAPREDUCTIONFUNCTIONTESTC, "$Id$");

#define OVERLAPREDUCTIONFUNCTIONTESTC_LENGTH    8
#define OVERLAPREDUCTIONFUNCTIONTESTC_F0        0.0
#define OVERLAPREDUCTIONFUNCTIONTESTC_DELTAF    80.0
#define OVERLAPREDUCTIONFUNCTIONTESTC_TOL       1e-6
#define OVERLAPREDUCTIONFUNCTIONTESTC_Z         5e+6

#define OVERLAPREDUCTIONFUNCTIONTESTC_TRUE     1
#define OVERLAPREDUCTIONFUNCTIONTESTC_FALSE    0

extern char *optarg;
extern int   optind;

/* int lalDebugLevel = LALMSGLVL3; */
int lalDebugLevel = LALNDEBUG;
BOOLEAN optVerbose    = OVERLAPREDUCTIONFUNCTIONTESTC_FALSE;
REAL8 optDeltaF     = -1;
UINT4 optLength     = 0;
REAL8 optF0       = 0.0;
UINT4  optDetector1 = LALNumCachedDetectors;
UINT4  optDetector2 = LALNumCachedDetectors;
CHAR optFile[LALNameLength] = "";

static void
Usage (const char *program, int exitflag);

static void
ParseOptions (int argc, char *argv[]);

/***************************** <lalErrTable file="OverlapReductionFunctionTestCE"> */
#define OVERLAPREDUCTIONFUNCTIONTESTC_ENOM 0
#define OVERLAPREDUCTIONFUNCTIONTESTC_EARG 1
#define OVERLAPREDUCTIONFUNCTIONTESTC_ECHK 2
#define OVERLAPREDUCTIONFUNCTIONTESTC_EFLS 3
#define OVERLAPREDUCTIONFUNCTIONTESTC_EUSE 4
#define OVERLAPREDUCTIONFUNCTIONTESTC_MSGENOM "Nominal exit"
#define OVERLAPREDUCTIONFUNCTIONTESTC_MSGEARG "Error parsing command-line arguments"
#define OVERLAPREDUCTIONFUNCTIONTESTC_MSGECHK "Error checking failed to catch bad data"
#define OVERLAPREDUCTIONFUNCTIONTESTC_MSGEFLS "Incorrect answer for valid data"
#define OVERLAPREDUCTIONFUNCTIONTESTC_MSGEUSE "Bad user-entered data"
/***************************** </lalErrTable> */

int main( int argc, char *argv[] )
{
  static LALStatus                status;
  
  OverlapReductionFunctionParameters   parameters;
  REAL4FrequencySeries     overlap;
  
  REAL4FrequencySeries     dummyOutput;

  REAL4                *tempPtr;

  const REAL4 expectedOutputDataData[8] = {1.0, .2113956922,
					   -.1372693019, .9606085997e-2,
					   .6120349226e-1, -.5375432760e-1,
					   .3700960588e-2, .3519147931e-1};
  LALDetectorPair     detectors;
  const LALDetector         plusAtOrigin  = { {0.0, 0.0, 0.0},
					      { {0.5, 0.0, 0.0},
						{0.0,-0.5, 0.0},
						{0.0, 0.0, 0.0} },
					      LALDETECTORTYPE_ABSENT,
					      { "", 0, 0, 0, 0, 0, 0, 0}
                                             };
  /*   LALDetector         crossAtOrigin = { {0.0, 0.0, 0.0},
                                        { {0.0, 0.5, 0.0},
                                          {0.5, 0.0, 0.0},
                                          {0.0, 0.0, 0.0} },
                                       LALDETECTORTYPE_ABSENT,
					      { "", 0, 0, 0, 0, 0, 0, 0}
                                             };
  */
  const LALDetector         plusOnZAxis   = { {0.0, 0.0, OVERLAPREDUCTIONFUNCTIONTESTC_Z},
					      { {0.5, 0.0, 0.0},
						{0.0,-0.5, 0.0},
						{0.0, 0.0, 0.0} },
					      LALDETECTORTYPE_ABSENT,
					      { "", 0, 0, 0, 0, 0, 0, 0}
                                             };
  const LALDetector         crossOnZAxis  = { {0.0, 0.0, OVERLAPREDUCTIONFUNCTIONTESTC_Z},
					      { {0.0, 0.5, 0.0},
						{0.5, 0.0, 0.0},
						{0.0, 0.0, 0.0} },
					      LALDETECTORTYPE_ABSENT,
					      { "", 0, 0, 0, 0, 0, 0, 0}
                                             };
  UINT4 i;
  REAL4 overlapVal, f;
  INT4 code;
  

  /* define valid parameters */
  
  parameters.length   = OVERLAPREDUCTIONFUNCTIONTESTC_LENGTH;
  parameters.f0       = OVERLAPREDUCTIONFUNCTIONTESTC_F0;
  parameters.deltaF   = OVERLAPREDUCTIONFUNCTIONTESTC_DELTAF;

  overlap.data = NULL;

  dummyOutput.data = NULL;

  detectors.detectorOne = detectors.detectorTwo = plusAtOrigin;

  ParseOptions( argc, argv );

  LALSCreateVector(&status, &(overlap.data), OVERLAPREDUCTIONFUNCTIONTESTC_LENGTH);
  if ( ( code = CheckStatus(&status, 0 , "",
			    OVERLAPREDUCTIONFUNCTIONTESTC_EFLS,
			    OVERLAPREDUCTIONFUNCTIONTESTC_MSGEFLS) ) )
  {
    return code;
  }

  /* TEST INVALID DATA HERE ------------------------------------------ */
#ifndef LAL_NDEBUG
  if ( ! lalNoDebug )
  {
    /* test behavior for null pointer to real frequency series for output */
    LALOverlapReductionFunction(&status, NULL, &detectors, &parameters);
    if ( ( code = CheckStatus(&status, STOCHASTICCROSSCORRELATIONH_ENULLPTR, 
			      STOCHASTICCROSSCORRELATIONH_MSGENULLPTR,
			      OVERLAPREDUCTIONFUNCTIONTESTC_ECHK,
			      OVERLAPREDUCTIONFUNCTIONTESTC_MSGECHK) ) )
    {
      return code;
    }
    printf("  PASS: null pointer to output series results in error:       \n\"%s\"\n", STOCHASTICCROSSCORRELATIONH_MSGENULLPTR);

    /* test behavior for null pointer to input structure */
    LALOverlapReductionFunction(&status, &overlap, NULL, &parameters);
    if ( ( code = CheckStatus(&status, STOCHASTICCROSSCORRELATIONH_ENULLPTR, 
			      STOCHASTICCROSSCORRELATIONH_MSGENULLPTR,
			      OVERLAPREDUCTIONFUNCTIONTESTC_ECHK,
			      OVERLAPREDUCTIONFUNCTIONTESTC_MSGECHK) ) )
    {
      return code;
    }
    printf("  PASS: null pointer to input structure results in error:       \n\"%s\"\n", STOCHASTICCROSSCORRELATIONH_MSGENULLPTR);


    /* test behavior for null pointer to parameter structure */
    LALOverlapReductionFunction(&status, &overlap, &detectors, NULL);
    if ( ( code = CheckStatus(&status, STOCHASTICCROSSCORRELATIONH_ENULLPTR, 
			      STOCHASTICCROSSCORRELATIONH_MSGENULLPTR,
			      OVERLAPREDUCTIONFUNCTIONTESTC_ECHK,
			      OVERLAPREDUCTIONFUNCTIONTESTC_MSGECHK) ) )
    {
      return code;
    }
    printf("  PASS: null pointer to parameter structure results in error:       \n\"%s\"\n", STOCHASTICCROSSCORRELATIONH_MSGENULLPTR);


    /* test behavior for null pointer to data member of real frequency
       series for output */
    LALOverlapReductionFunction(&status, &dummyOutput, &detectors, &parameters);
    if ( ( code = CheckStatus(&status, STOCHASTICCROSSCORRELATIONH_ENULLPTR, 
			      STOCHASTICCROSSCORRELATIONH_MSGENULLPTR,
			      OVERLAPREDUCTIONFUNCTIONTESTC_ECHK,
			      OVERLAPREDUCTIONFUNCTIONTESTC_MSGECHK) ) )
    {
      return code;
    }
    printf("  PASS: null pointer to data member of output series results in error:       \n\"%s\"\n",
           STOCHASTICCROSSCORRELATIONH_MSGENULLPTR);


    /* Create a vector for testing null data-data pointer */
    LALSCreateVector(&status, &(dummyOutput.data), OVERLAPREDUCTIONFUNCTIONTESTC_LENGTH);
    if ( ( code = CheckStatus(&status, 0 , "",
			      OVERLAPREDUCTIONFUNCTIONTESTC_EFLS,
			      OVERLAPREDUCTIONFUNCTIONTESTC_MSGEFLS) ) )
    {
      return code;
    }
    tempPtr = dummyOutput.data->data;
    dummyOutput.data->data = NULL;

    /* test behavior for null pointer to data member of data member of
       real frequency series for output */
    LALOverlapReductionFunction(&status, &dummyOutput, &detectors, &parameters);
    if ( ( code = CheckStatus(&status, STOCHASTICCROSSCORRELATIONH_ENULLPTR, 
			      STOCHASTICCROSSCORRELATIONH_MSGENULLPTR,
			      OVERLAPREDUCTIONFUNCTIONTESTC_ECHK,
			      OVERLAPREDUCTIONFUNCTIONTESTC_MSGECHK) ) )
    {
      return code;
    }
    printf("  PASS: null pointer to data member of data member of output series results in error:       \n\"%s\"\n",
           STOCHASTICCROSSCORRELATIONH_MSGENULLPTR);

    /* clean up */
    
    dummyOutput.data->data = tempPtr;
    LALSDestroyVector(&status, &(dummyOutput.data));
    if ( ( code = CheckStatus(&status, 0 , "", 
			      OVERLAPREDUCTIONFUNCTIONTESTC_EFLS,
			      OVERLAPREDUCTIONFUNCTIONTESTC_MSGEFLS) ) )
    {
      return code;
    }

    /* test behavior for length parameter equal to zero */
    parameters.length = 0;
    LALOverlapReductionFunction(&status, &overlap, &detectors, &parameters);
    if ( ( code = CheckStatus(&status, STOCHASTICCROSSCORRELATIONH_EZEROLEN,
			      STOCHASTICCROSSCORRELATIONH_MSGEZEROLEN,
			      OVERLAPREDUCTIONFUNCTIONTESTC_ECHK,
			      OVERLAPREDUCTIONFUNCTIONTESTC_MSGECHK) ) )
    {
      return code;
    }
    printf("  PASS: zero length parameter results in error:       \n\"%s\"\n", 
	   STOCHASTICCROSSCORRELATIONH_MSGEZEROLEN);
    /* assign valid length parameter */
    parameters.length = OVERLAPREDUCTIONFUNCTIONTESTC_LENGTH;

    /* test behavior for frequency spacing less than or equal to zero */
    parameters.deltaF = -1;
    LALOverlapReductionFunction(&status, &overlap, &detectors, &parameters);
    if ( ( code = CheckStatus(&status, STOCHASTICCROSSCORRELATIONH_ENONPOSDELTAF,
			      STOCHASTICCROSSCORRELATIONH_MSGENONPOSDELTAF,
			      OVERLAPREDUCTIONFUNCTIONTESTC_ECHK,
			      OVERLAPREDUCTIONFUNCTIONTESTC_MSGECHK) ) )
    {
      return code;
    }
    printf("  PASS: negative frequency spacing results in error:       \n\"%s\"\n",
	   STOCHASTICCROSSCORRELATIONH_MSGENONPOSDELTAF);

    parameters.deltaF = 0;
    LALOverlapReductionFunction(&status, &overlap, &detectors, &parameters);
    if ( ( code = CheckStatus(&status, STOCHASTICCROSSCORRELATIONH_ENONPOSDELTAF,
			      STOCHASTICCROSSCORRELATIONH_MSGENONPOSDELTAF,
			      OVERLAPREDUCTIONFUNCTIONTESTC_ECHK,
			      OVERLAPREDUCTIONFUNCTIONTESTC_MSGECHK) ) )
    {
        return code;
    }
    printf("  PASS: zero frequency spacing results in error:       \n\"%s\"\n",
	   STOCHASTICCROSSCORRELATIONH_MSGENONPOSDELTAF);
    /* assign valid frequency spacing */
    parameters.deltaF = OVERLAPREDUCTIONFUNCTIONTESTC_DELTAF;
  }

#endif /* LAL_NDEBUG */

  /* test behavior for negative start frequency */
  parameters.f0 = -20.0;
  LALOverlapReductionFunction(&status, &overlap, &detectors, &parameters);
  if ( ( code = CheckStatus(&status, STOCHASTICCROSSCORRELATIONH_ENEGFMIN,
			    STOCHASTICCROSSCORRELATIONH_MSGENEGFMIN,
			    OVERLAPREDUCTIONFUNCTIONTESTC_ECHK,
			    OVERLAPREDUCTIONFUNCTIONTESTC_MSGECHK) ) )
  {
    return code;
  }
  printf("  PASS: negative start frequency results in error:\n       \"%s\"\n",
         STOCHASTICCROSSCORRELATIONH_MSGENEGFMIN);
  
  /* reassign valid start frequency */
  parameters.f0 = OVERLAPREDUCTIONFUNCTIONTESTC_F0;

  /* test behavior for length of data member of real frequency series 
     for output not equal to length specified in input parameters */
  parameters.length += 1;
  LALOverlapReductionFunction(&status, &overlap, &detectors, &parameters);
  if ( ( code = CheckStatus(&status, STOCHASTICCROSSCORRELATIONH_EMMLEN, 
			    STOCHASTICCROSSCORRELATIONH_MSGEMMLEN,
			    OVERLAPREDUCTIONFUNCTIONTESTC_ECHK,
			    OVERLAPREDUCTIONFUNCTIONTESTC_MSGECHK) ) )
  {
    return code;
  }
  printf("  PASS: mismatch between length of output series and length parameter results in error:       \n\"%s\"\n", STOCHASTICCROSSCORRELATIONH_MSGEMMLEN);
  /* reassign valid length to data member of dummy output */
  parameters.length -= 1;
  
  /* TEST VALID DATA HERE -------------------------------------------- */


  /* generate overlap red fcn */
  LALOverlapReductionFunction(&status, &overlap, &detectors, &parameters);
  if ( ( code = CheckStatus(&status,0, "",
			    OVERLAPREDUCTIONFUNCTIONTESTC_EFLS,
			    OVERLAPREDUCTIONFUNCTIONTESTC_MSGEFLS) ) )
  {
    return code;
  }

  /* test values */

  overlapVal = 1.0;
  for (i=0; i<OVERLAPREDUCTIONFUNCTIONTESTC_LENGTH; ++i) 
  {
    f = i * OVERLAPREDUCTIONFUNCTIONTESTC_DELTAF;
    if (optVerbose) {
      printf("gamma(%f Hz)=%f, should be %f\n",
             f, overlap.data->data[i], overlapVal);
    }
    if ( (overlap.data->data[i] - overlapVal) &&
         abs((overlap.data->data[i] - overlapVal)/overlapVal) 
         > OVERLAPREDUCTIONFUNCTIONTESTC_TOL )
    {
      printf("  FAIL: Valid data test #1 (coincident, coaligned IFOs)\n");
      return OVERLAPREDUCTIONFUNCTIONTESTC_EFLS;
    }
  }
  printf("  PASS: Valid data test #1 (coincident, coaligned IFOs)\n");

  /* change parameters */
  detectors.detectorTwo = plusOnZAxis;

  /* generate overlap red fcn */
  LALOverlapReductionFunction(&status, &overlap, &detectors, &parameters);
  if ( ( code = CheckStatus(&status,0, "", 
			    OVERLAPREDUCTIONFUNCTIONTESTC_EFLS,
			    OVERLAPREDUCTIONFUNCTIONTESTC_MSGEFLS) ) )
  {
    return code;
  }

  /* test values */

  for (i=0; i<OVERLAPREDUCTIONFUNCTIONTESTC_LENGTH; ++i) 
  {
    f = i * OVERLAPREDUCTIONFUNCTIONTESTC_DELTAF;
    if (optVerbose) {
      printf("gamma(%f Hz)=%f, should be %f\n",
             f, overlap.data->data[i], expectedOutputDataData[i]);
    }
    if ( (overlap.data->data[i] - expectedOutputDataData[i]) &&
         abs((overlap.data->data[i] - expectedOutputDataData[i])
	     / expectedOutputDataData[i])
	 > OVERLAPREDUCTIONFUNCTIONTESTC_TOL )
    {
      printf("  FAIL: Valid data test #2 (coaligned, non-coincident IFOs)\n");
      return OVERLAPREDUCTIONFUNCTIONTESTC_EFLS;
    }
  }
  printf("  PASS: Valid data test #2 (coaligned, non-coincident IFOs)\n");
  /* change parameters */
  detectors.detectorTwo = crossOnZAxis;

  /* generate overlap red fcn */
  LALOverlapReductionFunction(&status, &overlap, &detectors, &parameters);
  if ( ( code = CheckStatus(&status,0, "",
			    OVERLAPREDUCTIONFUNCTIONTESTC_EFLS,
			    OVERLAPREDUCTIONFUNCTIONTESTC_MSGEFLS) ) )
  {
    return code;
  }

  /* test values */

  overlapVal = 0.0;
  for (i=0; i<OVERLAPREDUCTIONFUNCTIONTESTC_LENGTH; ++i) 
  {
    f = i * OVERLAPREDUCTIONFUNCTIONTESTC_DELTAF;
    if (optVerbose) {
      printf("gamma(%f Hz)=%f, should be %f\n",
             f, overlap.data->data[i], overlapVal);
    }
    if ( (overlap.data->data[i] - overlapVal) &&
         abs(overlap.data->data[i] - overlapVal)
	 > OVERLAPREDUCTIONFUNCTIONTESTC_TOL )
    {
      printf("  FAIL: Valid data test #3 (misaligned IFOs)\n");
      return OVERLAPREDUCTIONFUNCTIONTESTC_EFLS;
    }
  }
  printf("  PASS: Valid data test #3 (misaligned IFOs)\n");

  /* change parameters */
  /* 
     detectors.detectorTwo = plusOnZAxis; 
   */

  /* clean up valid data */
  LALSDestroyVector(&status, &(overlap.data));
  if ( ( code = CheckStatus(&status,0, "",
			    OVERLAPREDUCTIONFUNCTIONTESTC_EFLS,
			    OVERLAPREDUCTIONFUNCTIONTESTC_MSGEFLS) ) )
  {
    return code;
  }

  LALCheckMemoryLeaks();

  printf("PASS: all tests\n");

  if (optFile[0]) {
    parameters.length = optLength;
    parameters.f0 = optF0;
    parameters.deltaF = optDeltaF;

    if (optDetector1 < LALNumCachedDetectors)
    {
      detectors.detectorOne = lalCachedDetectors[optDetector1];
    }
    else {
      return OVERLAPREDUCTIONFUNCTIONTESTC_EUSE;
    }
    
    if (optDetector2 < LALNumCachedDetectors)
    {
      detectors.detectorTwo = lalCachedDetectors[optDetector2];
    }
    else {
      return OVERLAPREDUCTIONFUNCTIONTESTC_EUSE;
    }

    LALSCreateVector(&status, &(overlap.data), optLength);
    if ( ( code = CheckStatus(&status, 0 , "", 
			      OVERLAPREDUCTIONFUNCTIONTESTC_EUSE,
			      OVERLAPREDUCTIONFUNCTIONTESTC_MSGEUSE) ) )
    {
      return code;
    }
    LALOverlapReductionFunction(&status, &overlap, &detectors, &parameters);
    if ( ( code = CheckStatus(&status, 0 , "", 
			      OVERLAPREDUCTIONFUNCTIONTESTC_EUSE,
			      OVERLAPREDUCTIONFUNCTIONTESTC_MSGEUSE) ) )
    {
      return code;
    }
    LALSPrintFrequencySeries( &overlap, optFile );

    printf("======== Overlap Reduction Function Written to File %s ========\n", optFile);

    LALSDestroyVector(&status, &(overlap.data));
    if ( ( code = CheckStatus(&status, 0 , "", 
			      OVERLAPREDUCTIONFUNCTIONTESTC_EUSE,
			      OVERLAPREDUCTIONFUNCTIONTESTC_MSGEUSE) ) )
    {
      return code;
    }

    LALCheckMemoryLeaks();
  }

  return OVERLAPREDUCTIONFUNCTIONTESTC_ENOM;

}


/*
 * Usage ()
 *
 * Prints a usage message for program program and exits with code exitcode.
 *
 */
static void
Usage (const char *program, int exitcode)
{
  INT4 i;

  fprintf (stderr, "Usage: %s [options]\n", program);
  fprintf (stderr, "Options:\n");
  fprintf (stderr, "  -h             print this message\n");
  fprintf (stderr, "  -q             quiet: run silently\n");
  fprintf (stderr, "  -v             verbose: print extra information\n");
  fprintf (stderr, "  -d level       set lalDebugLevel to level\n");
  fprintf (stderr, "  -s siteID1     calculate overlap red fcn for site siteID1\n");
  fprintf (stderr, "  -t siteID2       with site siteID2\n");
  for (i=0; i<LALNumCachedDetectors; ++i)
  {
    fprintf (stderr, "                   %d = %s\n",
             i, lalCachedDetectors[i].frDetector.name);
  }
  fprintf (stderr, "  -f f0          set start frequency to f0\n");
  fprintf (stderr, "  -e deltaF      set frequency spacing to deltaF\n");
  fprintf (stderr, "  -n length      set number of points in frequency series to length\n");
  fprintf (stderr, "  -o filename    print overlap reduction function to file filename\n");
  exit (exitcode);
}

/*
 * ParseOptions ()
 *
 * Parses the argc - 1 option strings in argv[].
 *
 */
static void
ParseOptions (int argc, char *argv[])
{
  while (1)
  {
    int c = -1;

    c = getopt (argc, argv, "hqvd:s:t:f:e:n:o:");
    if (c == -1)
    {
      break;
    }

    switch (c)
    {
      case 'o': /* specify output file */
        strncpy (optFile, optarg, LALNameLength);
        break;
        
      case 'n': /* specify number of points in frequency series */
        optLength = atoi (optarg);
        break;
        
      case 'e': /* specify frequency resolution */
        optDeltaF = atof (optarg);
        break;
        
      case 'f': /* specify start frequency */
        optF0 = atof (optarg);
        break;

      case 's': /* specify detector #1 */
        optDetector1 = atoi (optarg);
	break;

      case 't': /* specify detector #2 */
        optDetector2 = atoi (optarg);
	break;

      case 'd': /* set debug level */
        lalDebugLevel = atoi (optarg);
        break;

      case 'v': /* optVerbose */
        optVerbose = OVERLAPREDUCTIONFUNCTIONTESTC_TRUE;
        break;

      case 'q': /* quiet: run silently (ignore error messages) */
        freopen ("/dev/null", "w", stderr);
        freopen ("/dev/null", "w", stdout);
        break;

      case 'h':
        Usage (argv[0], 0);
        break;

      default:
        Usage (argv[0], 1);
    }

  }

  if (optind < argc)
  {
    Usage (argv[0], 1);
  }

  return;
}
