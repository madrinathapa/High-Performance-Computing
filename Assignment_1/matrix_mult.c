#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define PI 3.14159265
int main()
{
    int rows = 183;

    //Allocate memories for matrix, vector and result vector
    float *matrix = (float *)calloc(rows * rows, sizeof(float));
    float *vec = (float *)malloc(rows * 1 * sizeof(float));
    float *result = (float *)calloc(rows ,sizeof(float));

    FILE *fptr;
    char ch;
    int i, j, count=0,r=0,c=0;

    //Read file
    if ((fptr = fopen("input.mtx","r")) == NULL){
        printf("Error! opening file");
        exit(1);
    }

    char *line = NULL;
    size_t len = 0;
    double val=0.0;


    while ((getline(&line, &len, fptr)) != -1) {
        if(count>=2){
            char * pch = strtok (line," ");
            r=0; c=0;
            while (pch != NULL)
            {
              if(r== 0){
                r = atoi(pch);
                r--;
              }else if(c == 0){
                c = atoi(pch);
                c--;
              }else{
                  val = strtod(pch,NULL);
              }
              pch = strtok (NULL, " ");
            }
              //set the value in matrix
              *(matrix + r*rows + c) = val;
          }
            count++;
         }
  fclose(fptr);
  
  clock_t start_t, end_t, total_t;
  
  start_t = time(NULL);
  //Set the valu in vector
  for(i = 0; i < rows; i++){
  	*(vec+i) = sin((2*PI*i)/182);
  }
  end_t = time(NULL);

  total_t +=(double)(end_t - start_t);

  start_t = time(NULL);
  // Set the value in result vector
  for(i=0;i<rows;i++){
    for(j=0;j<rows;j++){
      *(result+i)+=(*(matrix + i*rows + j)) * (*(vec+j));
    }
  }
  end_t = time(NULL);

  total_t += (double)(end_t - start_t);

  float sum=0.0, l2;

  start_t = time(NULL);
  for(i = 0; i < rows; i++){
      sum += pow(fabs(*(result+i)),2);
  }
  l2 = sqrt(sum);
  end_t = time(NULL);
  total_t += (double)(end_t - start_t);
  printf("Time taken for the operation:%f \n",total_t);
  printf("l2: %lf\n", l2);
  return 0;
}

