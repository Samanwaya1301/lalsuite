#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <math.h>
#include <zlib.h>

#ifdef SKYMAP_PNG
#include <png.h>
#endif

#include <lal/LALConstants.h>
#include <lal/Skymap.h>

#include <lal/FrameData.h>
#include <lal/FrameStream.h>
#include <lal/LALFrameIO.h>
#include <lal/LALDatatypes.h>
#include <lal/AVFactories.h>
#include <lal/Date.h>

#include <lal/LIGOLwXMLRead.h>
#include <lal/LIGOMetadataTables.h>

#define max(A,B) (((A) > (B)) ? (A) : (B))

/*
 *  File names for input and output
 */
const char* h1_frame_file = 0;
const char* l1_frame_file = 0;
const char* v1_frame_file = 0;

char* xml_file[3] = { 0, 0, 0};

const char* output_file = "skymap.txt";

char* event_id = 0;
/*
 *  Time of the trigger to be analyzed
 */
double w[3] = { 1, 1, 1 };
/*
 *  Resolution of output sky map
 */
int ra_res = 512;
int dec_res = 256;

/*
 *  Sampling frequency of analysis, which determines sky tiling
 */
int frequency = 0;

/*
 *  Number of samples to analyze
 */
int samples = 512;

/*
 *  Array of pointers to the matched-filtered data z[t]
 * 
 *  x[0] = real(z_H)
 *  x[1] = real(z_L)
 *  x[2] = real(z_V)
 *  x[3] = imag(z_H)
 *  x[4] = imag(z_L)
 *  x[5] = imag(z_V)
 */
double* x[6] = { 0, 0, 0, 0, 0, 0 };
/*
 * Orientation of the earth at the event time
 */
double greenwich = 0;
/*
 * Functions
 */
void load_metadata(char* file, int detector);
void load_data(int detector, const char* file, const char* initial);
void dump_data_to_file(FILE*);
void analyze(void);

int main(int argc, char** argv)
{
    int c;
    
    while (1)
    {
        static struct option long_options[] =
            {
                {"h1-frame-file", required_argument, 0, 'h'},
                {"l1-frame-file", required_argument, 0, 'l'},
                {"v1-frame-file", required_argument, 0, 'v'},
                {"output-file", required_argument, 0, 'o'},
                {"ra-res", required_argument, 0, 'a'},
                {"dec-res", required_argument, 0, 'd'},
                {"h1-xml-file", required_argument, 0, 't'},
                {"l1-xml-file", required_argument, 0, 's'},
                {"v1-xml-file", required_argument, 0, 'r'},
                {"event-id", required_argument, 0, 'e'},
                {"sample-rate", required_argument, 0, 'f'},
                {0, 0, 0, 0}
            };
        int option_index = 0;
        c = getopt_long_only(argc, argv, "h:l:v:o:a:d:t:s:r:e:f:", long_options, &option_index);
        if (c == -1)
            break;
        
        switch (c)
        {
            case 'h':
		if (strcmp(optarg,"none")) h1_frame_file = optarg;
                break;
            case 'l':
		if (strcmp(optarg,"none")) l1_frame_file = optarg;
                break;
            case 'v':
                if (strcmp(optarg,"none")) v1_frame_file = optarg;
                break;
            case 'o':
                output_file = optarg;
                break;
            case 'a':
                ra_res = atoi(optarg);
                break;
            case 'd':
                dec_res = atoi(optarg);
                break;
            case 't':
                if (strcmp(optarg,"none")) xml_file[0] = (optarg);
                break;
	    case 's':
	        if (strcmp(optarg,"none")) xml_file[1] = (optarg);
		break;
            case 'r':
	        if (strcmp(optarg,"none")) xml_file[2] = (optarg);
                break;
            case 'e':
                event_id = optarg;
                break;
            case 'f':
                frequency = atoi(optarg);
                break;
            default:
                fprintf(stderr, "unknown error while parsing options\n");
                    exit(1);
        }
    }
    
    if (optind < argc)
    {
        fprintf(stderr, "error: extraneous command line argument ");
        while (optind < argc)
        {
            fprintf(stderr, "%s\n", argv[optind++]);
        }
        exit(1);
    }
    
    /* check sanity */
    if (!(h1_frame_file || l1_frame_file || v1_frame_file))
    {
        fprintf(stderr, "error: Supply at least one of --h1-frame-file, --l1-frame-file or --v1-frame-file\n");
        exit(1);
    }
    if (h1_frame_file && !xml_file[0])
    {
        fprintf(stderr, "error: Supply --h1-xml-file to match --h1-frame-file\n");
        exit(1);
    }
    if (l1_frame_file && !xml_file[1])
    {
        fprintf(stderr, "error: Supply --l1-xml-file to match --l1-frame-file\n");
        exit(1);
    }
    if (v1_frame_file && !xml_file[2])
    {
        fprintf(stderr, "error: Supply --v1-xml-file to match --v1-frame-file\n");
        exit(1);
    }
    if (!h1_frame_file && xml_file[0])
    {
        fprintf(stderr, "error: Supply --h1-frame-file to match --h1-xml-file\n");
        exit(1);
    }
    if (!l1_frame_file && xml_file[1])
    {
        fprintf(stderr, "error: Supply --l1-frame-file to match --l1-xml-file\n");
        exit(1);
    }
    if (!v1_frame_file && xml_file[2])
    {
        fprintf(stderr, "error: Supply --v1-frame-file to match --v1-xml-file\n");
        exit(1);
    }
    
    if (frequency <= 0)
    {
        fprintf(stderr, "error: Supply positive integer Hertz --sample-rate\n");
        exit(1);
    }

    /* examine one second of data around the injection */
    samples = frequency;
    /*
     * Load metadata from XML files
     */
    load_metadata(xml_file[0], 0);
    load_metadata(xml_file[1], 1);
    load_metadata(xml_file[2], 2);
    /*
     * Load z[t] data from frame files
     */
    load_data(0, h1_frame_file, "H");
    load_data(1, l1_frame_file, "L");
    load_data(2, v1_frame_file, "V");

    analyze();
    
    return 0;
}

void load_metadata(char* file, int detector)
{
    if (file)
    {
        SnglInspiralTable* a = 0;
        LALSnglInspiralTableFromLIGOLw(&a, file, 0, 1);
        if (!a)
        {
            fprintf(stderr, "error: failed to read single inspiral table from file %s\n", file);
            exit(1);
        }
        w[detector] = sqrt(a->sigmasq);
        greenwich = fmod(XLALGreenwichMeanSiderealTime(&(a->end_time)), LAL_TWOPI);
        fprintf(stderr, "GPS %d -> GMS %e -> RAD %e \n", a->end_time.gpsSeconds, XLALGreenwichMeanSiderealTime(&(a->end_time)), greenwich);
    }
}

void load_data(int detector, const char* file, const char* initial)
{
    if (file)
    {
        /* 
         *  Read the frame file here
         */

        FrStream *stream = NULL;
        COMPLEX8TimeSeries H1series;
        int i;

        sprintf(H1series.name,"%s1:CBC-CData_%s", initial, event_id);
        stream = XLALFrOpen("./", file);
        if (!stream)
        {
            fprintf(stderr, "error: failed to open FrStream from file %s\n", H1series.name);
            exit(1);
        }
        
        H1series.data = XLALCreateCOMPLEX8Vector(samples);
        if (!H1series.data)
        {
            fprintf(stderr, "error: failed to create COMPLEX8 vector\n");
            exit(1);
        }
        XLALFrGetCOMPLEX8TimeSeries(&H1series,stream);
        XLALFrClose(stream);

        /*
         *  Allocate memory to repack the data in
         */

        /* real, or waveform one */
        x[detector] = (double*) malloc(samples * sizeof(double));
        /* complex, or waveform two */
        x[detector + 3] = (double*) malloc(samples * sizeof(double));     

        for (i = 0; i != samples; ++i)
        {
            x[detector    ][i] = H1series.data->data[i].re;
            x[detector + 3][i] = H1series.data->data[i].im;
        }
    }
}

void dump_data_to_file(FILE* h)
{
    int i;
    for (i = 0; i != samples; ++i)
    {
        fprintf(h, "%e %e %e %e %e %e %e\n", ((double) i)/frequency, x[0][i], x[3][i], x[1][i], x[4][i], x[2][i], x[5][i]);
    }
}
#define NSIGMA 11

void analyze(void)
{

    XLALSkymapPlanType* plan;
    double s[NSIGMA];
    int i, j;
    double* raw;
    double* accumulator;
    int begin[3], end[3];

    begin[0] = 0;
    begin[1] = begin[0];
    begin[2] = begin[0];

    end[0] = samples;
    end[1] = end[0];
    end[2] = end[0];

    /* 
     *  The characteristic size of the signal 
     */

    /*fprintf(stderr, "w: %e %e %e\n", w[0], w[1], w[2]);*/

    s[0]  =    1;
    s[1]  =    4;
    s[2]  =    16;
    s[3]  =    64;
    s[4]  =   256;
    s[5]  =   1024;
    s[6]  =   1./4;
    s[7]  =  1./16;
    s[8]  =  1./64;
    s[9]  =  1./256;
    s[10] = 1./1024;
     
    /*   
     *  the sky tiles implied by the frequency) 
     */
    plan = XLALSkymapConstructPlan(frequency);  
    /*
     *  Allocate a chunk of memory tto hold the sky map in the internal 
     *  timing format
     */
    raw = (double*) calloc(plan->pixelCount, sizeof(double));
    accumulator = (double*) calloc(plan->pixelCount, sizeof(double));
    /*
     *  Generate the skymap
     */
    XLALSkymapEllipticalHypothesis(plan, accumulator, s[0], w, begin, end, x, 0);
    for (i = 1; i != NSIGMA; ++i)
    {

        XLALSkymapEllipticalHypothesis(plan, raw, s[i], w, begin, end, x, 0);
        XLALSkymapSum(plan, accumulator, accumulator, raw);
    }

    free(raw);
    raw = accumulator;

    {   /*  
         *  Get the mode 
         */ 
        double thetaphi[2];
        XLALSkymapModeThetaPhi(plan, raw, thetaphi);
        while (thetaphi[1] < 0)
            thetaphi[1] += LAL_TWOPI;
        printf("%e %e\n", thetaphi[0], thetaphi[1]);
    }
    
    {   /*  
         *  Render the timing-format sky map to a more friendly coordinate 
         *  system
         */
        double* render;
        int m = dec_res;
        int n = ra_res;
        double maximum;
        render = (double*) malloc(m * n * sizeof(double));
        XLALSkymapRenderEquirectangular(m, n, render, plan, raw);
        /*XLALSkymapRenderMollweide(m, n, render, plan, raw);*/

        maximum = render[0];
        for (j = 1; j != m * n; ++j)
        {
            if (render[j] > maximum)
            {
                maximum = render[j];
            }
        }
        for (j = 0; j != m * n; ++j)
        {
            render[j] -= maximum;
        }



        {
            /*
             *  Write a gzipped ascii file describing the sky map
             */
            gzFile* h = NULL;
            h = gzopen(output_file, "wb");
            if (h == NULL) 
            {
                fprintf(stderr,"cannot gzopen() %s\n",output_file);
                exit(1);
            }
            for (j = 0; j != n; ++j)
            {
                double phi, ra;
                phi = (LAL_TWOPI * (j + 0.5)) / n;
                ra = fmod(phi + greenwich, LAL_TWOPI);
                while (ra < 0)
                    ra += LAL_TWOPI;
                while (ra >= LAL_TWOPI)
                    ra -= LAL_TWOPI;
                for (i = 0; i != m; ++i)
                {
                    double dec;
                    dec = LAL_PI_2 - (LAL_PI * (i + 0.5)) / m;
                    gzprintf(h, "%.10e %.10e %.10e\n", ra, dec, exp(render[i + m * j]));
                }
            }
            gzclose(h);
        }
        
        free(render);
        
    }
        
    free(raw);
        
    /*
     *  The plan has allocated memory, so we have to destroy it
     */
    XLALSkymapDestroyPlan(plan);
    
    /*
     *  Free the data
     */
    {
        for (i = 0; i != 6; ++i)
        {
            free(x[i]);
        }
    }

}

#ifdef SKYMAP_PNG
        {
            FILE* fp;
            png_structp png_ptr;
            png_infop info_ptr;
            
            unsigned char* row_ptr;
            int i;
            int j;
            double minimum;
            double maximum;
            
            fp = fopen(output_file, "wb");
            
            png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
            info_ptr = png_create_info_struct(png_ptr);
            png_init_io(png_ptr, fp);
            
            png_set_IHDR(png_ptr, info_ptr, ra_res, dec_res, 8, 
                PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE, 
                PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
            
            png_write_info(png_ptr, info_ptr);
            
            i = 0;
            while (isinf(render[i]))
                ++i;
            minimum = render[i];
            maximum = render[i];
            for (++i; i != dec_res * ra_res; ++i)
            {
                if (!isinf(render[i]))
                {
                    if (render[i] < minimum)
                        minimum = render[i];
                    if (render[i] > maximum)
                        maximum = render[i];
                }
            }
            
            printf("[%e, %e]\n", minimum, maximum);
            
            row_ptr = malloc(ra_res);
            
            for (i = 0; i != dec_res; ++i)
            {
                for (j = 0; j != ra_res; ++j)
                {
                    row_ptr[j] = (int) (255.0 * (render[i + j * dec_res] - minimum) / (maximum - minimum));
                    /*printf("%d\n",row_ptr[j]);*/
                }
                png_write_row(png_ptr, row_ptr);            
            }
            
            free(row_ptr);
            
            png_write_end(png_ptr, 0);
            png_destroy_write_struct(&png_ptr, &info_ptr);
            
            fclose(fp);
        }       
#endif   

#if 0

void load_test_data(const char* file);

void load_test_data(const char* file)
{
    FILE* h;
    int i;

    fprintf(stderr, "warning: overwriting with test data\n");

    for (i = 0; i != 6; ++i)
    {
	if(!(x[i] = (double*) malloc(samples * sizeof(double))))
        {
            fprintf(stderr, "malloc test data failed\n");
        }
    }

    if(!(h = fopen(file, "rt")))
    {
        fprintf(stderr, "open test file failed\n");
    }
    
    for (i = 0; i != samples; ++i)
    {
        int j;
    	fscanf(h, "%le %le %le %le %le %le\n", x[0] + i, x[3] + i, x[1] + i, x[4] + i, x[2] + i, x[5] + i);
        for(j=0; j!= 6;++j)
        {
           /*fprintf(stderr, "%e ", x[j][i]);*/
        }
        /*fprintf(stderr,"\n");*/
    }
    fclose(h);

    /*fprintf(stderr, "warning: overwrote with test data\n");*/
}
#endif

