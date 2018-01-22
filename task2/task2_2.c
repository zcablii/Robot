#include "simpletools.h"                      // Include simple tools
#include "abdrive.h"
#include <stdio.h>

void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}
 
// A function to implement bubble sort
void bubbleSort(int arr[], int n)
{
	for (int i = 0; i < n-1; i++)      
 
       // Last i elements are already in place   
		for (int j = 0; j < n-i-1; j++) 
		{
			if (arr[j] > arr[j+1])
			{
				swap(&arr[j], &arr[j+1]);
			  
			}
			for (int k =0; k < n; k++)
        	{
				printf("%d  ", arr[k]);
			}
        	    printf("\n");
		}
}
 
void printArray(int arr[], int size)
{
    int i;
    
		
}
 
int main()
{
    printf("Enter number of elements: ");
    int num;
    scanf("%d", &num);
    int arr[num];
    printf("Enter %i integers: \n", num);
    for (int i = 0; i<num; i++)
    {
        scanf("%d", &arr[i]);
    }
    bubbleSort(arr, num);
    return 0;
}
