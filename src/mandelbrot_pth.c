#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>

int NUM_THREADS;

double c_x_min;
double c_x_max;
double c_y_min;
double c_y_max;

double pixel_width;
double pixel_height;

int iteration_max = 200;
double escape_radius_squared = 4;

int image_size;
unsigned char **image_buffer;

int i_x_max;
int i_y_max;
int image_buffer_size;

int gradient_size = 16;
int colors[17][3] = {
                        {66, 30, 15},
                        {25, 7, 26},
                        {9, 1, 47},
                        {4, 4, 73},
                        {0, 7, 100},
                        {12, 44, 138},
                        {24, 82, 177},
                        {57, 125, 209},
                        {134, 181, 229},
                        {211, 236, 248},
                        {241, 233, 191},
                        {248, 201, 95},
                        {255, 170, 0},
                        {204, 128, 0},
                        {153, 87, 0},
                        {106, 52, 3},
                        {16, 16, 16},
                    };

static double rtclock() {
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    return t.tv_sec + t.tv_nsec * 1e-9;
}

typedef struct coordinates {
    int i_x;
    int i_y;
    double c_y;
} Coordinates;

void allocate_image_buffer(){
    int rgb_size = 3;
    image_buffer = (unsigned char **) malloc(sizeof(unsigned char *) * image_buffer_size);

    for(int i = 0; i < image_buffer_size; i++){
        image_buffer[i] = (unsigned char *) malloc(sizeof(unsigned char) * rgb_size);
    };
};

void init(int argc, char *argv[]){
    if(argc < 7){
        printf("usage: ./mandelbrot_pth c_x_min c_x_max c_y_min c_y_max image_size num_thr\n");
        printf("examples with image_size = 11500:\n");
        printf("    Full Picture:         ./mandelbrot_pth -2.5 1.5 -2.0 2.0 11500 1\n");
        printf("    Seahorse Valley:      ./mandelbrot_pth -0.8 -0.7 0.05 0.15 11500 2\n");
        printf("    Elephant Valley:      ./mandelbrot_pth 0.175 0.375 -0.1 0.1 11500 4\n");
        printf("    Triple Spiral Valley: ./mandelbrot_pth -0.188 -0.012 0.554 0.754 11500 8\n");
        exit(0);
    }
    else{
        sscanf(argv[1], "%lf", &c_x_min);
        sscanf(argv[2], "%lf", &c_x_max);
        sscanf(argv[3], "%lf", &c_y_min);
        sscanf(argv[4], "%lf", &c_y_max);
        sscanf(argv[5], "%d", &image_size);
        sscanf(argv[6], "%d", &NUM_THREADS);

        i_x_max           = image_size;
        i_y_max           = image_size;
        image_buffer_size = image_size * image_size;

        pixel_width       = (c_x_max - c_x_min) / i_x_max;
        pixel_height      = (c_y_max - c_y_min) / i_y_max;
    };
};

void update_rgb_buffer(int iteration, int x, int y){
    int color;

    if(iteration == iteration_max){
        image_buffer[(i_y_max * y) + x][0] = colors[gradient_size][0];
        image_buffer[(i_y_max * y) + x][1] = colors[gradient_size][1];
        image_buffer[(i_y_max * y) + x][2] = colors[gradient_size][2];
    }
    else{
        color = iteration % gradient_size;

        image_buffer[(i_y_max * y) + x][0] = colors[color][0];
        image_buffer[(i_y_max * y) + x][1] = colors[color][1];
        image_buffer[(i_y_max * y) + x][2] = colors[color][2];
    };
};

void write_to_file(){
    FILE * file;
    char * filename               = "output.ppm";
    char * comment                = "# ";

    int max_color_component_value = 255;

    file = fopen(filename,"wb");

    fprintf(file, "P6\n %s\n %d\n %d\n %d\n", comment,
            i_x_max, i_y_max, max_color_component_value);

    for(int i = 0; i < image_buffer_size; i++){
        fwrite(image_buffer[i], 1 , 3, file);
    };

    fclose(file);
};

void* compute_pixel(void* threadArg){
    Coordinates* coordinates = (Coordinates*) threadArg;

    double c_y      = coordinates->c_y;
    int i_y         = coordinates->i_y;
    int i_x         = coordinates->i_x;
    double c_x;

    double z_x;
    double z_y;

    double z_x_squared;
    double z_y_squared;
    int iteration;
    int chunk_size = image_size/NUM_THREADS;
    if (chunk_size == 0 && image_size != 0)
        chunk_size = 1;

    for(int j = i_x; j < i_x + chunk_size; j++){
        c_x         = c_x_min + j * pixel_width;

        z_x         = 0.0;
        z_y         = 0.0;

        z_x_squared = 0.0;
        z_y_squared = 0.0;

        for(iteration = 0;
            iteration < iteration_max && \
            ((z_x_squared + z_y_squared) < escape_radius_squared);
            iteration++){
            z_y         = 2 * z_x * z_y + c_y;
            z_x         = z_x_squared - z_y_squared + c_x;

            z_x_squared = z_x * z_x;
            z_y_squared = z_y * z_y;
        };

        update_rgb_buffer(iteration, j, i_y);
    }

    free(threadArg);
    pthread_exit(NULL);
}

void compute_mandelbrot(){
    double z_x;
    double z_y;
    double z_x_squared;
    double z_y_squared;
    int chunk_size = image_size/NUM_THREADS;
    pthread_attr_t attr;
    pthread_t xThreads[NUM_THREADS];

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    int i_x;
    int i_y;

    double c_x;
    double c_y;
    int busy_threads = NUM_THREADS;
    if (chunk_size == 0){
        chunk_size = 1;
        busy_threads = i_y_max;
    }

    for(i_y = 0; i_y < i_y_max; i_y++){
        c_y = c_y_min + i_y * pixel_height;
        i_x = 0;

        if(fabs(c_y) < pixel_height / 2){
            c_y = 0.0;
        };

        for(int i = 0; i < busy_threads; i++){
            Coordinates* coord = malloc(sizeof(Coordinates));
            coord->i_x = i_x;
            coord->i_y = i_y;
            coord->c_y = c_y;
            pthread_create(&xThreads[i], &attr, compute_pixel, (void*) coord);
            i_x += chunk_size;
        };

        for (int i = 0; i < busy_threads; i++) {
            pthread_join(xThreads[i], NULL);
        }
    };

    // caso necessário, cria o restante das threads que executarão tarefas vazias
    if (NUM_THREADS > busy_threads) {
        image_size = 0;
        for(int i = busy_threads; i < NUM_THREADS; i++){
            Coordinates* coord = malloc(sizeof(Coordinates));
            coord->i_x = i_x;
            coord->i_y = i_y;
            coord->c_y = c_y;
            pthread_create(&xThreads[i], &attr, compute_pixel, (void*) coord);
            i_x += chunk_size;
        };

        for (int i = i_y_max; i < NUM_THREADS; i++) {
            pthread_join(xThreads[i], NULL);
        }
    }
    pthread_attr_destroy(&attr);
};

int main(int argc, char *argv[]){
    init(argc, argv);

    allocate_image_buffer();
    
    double initial = rtclock();
    compute_mandelbrot();
    double final = rtclock();
    printf("%lf", final - initial);

    //write_to_file();

    return 0;
};
