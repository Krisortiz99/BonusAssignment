#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n)
{
	int larger;
	int temp;
	int l;
	int r;
	int i;
	for (i = n / 2 - 1; i >= 0; i--) {
		l = 2 * i + 1;
		r = 2 * i + 2;
		if (l < n && arr[l] > arr[i]) {
			larger = l;
		}
		if (r < n && arr[r] > arr[i]) {
			larger = r;
		}
		if (larger != i) {
			temp = arr[i];
			arr[i] = arr[larger];
			arr[larger] = temp;
		}
	}
	for (i = n - 1; i >= 0; i--) {
		int k = 0;
		l = 2 * k + 1;
		r = 2 * k + 2;
		if (l < n && arr[l] > arr[i]) {
			larger = l;
		}
		if (r < n && arr[r] > arr[i]) {
			larger = r;
		}
		if (larger != i) {
			temp = arr[i];
			arr[i] = arr[larger];
			arr[larger] = temp;
		}
		k++;
	}
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	int mid = size(pData) / 2;
	int i, j;
	int k = l;
	int size1 = mid - l + 1;
	int size2 = r - mid;
	int *L = (int*)Alloc(sizeof(int)*size1);
	int *R =(int*)Alloc(sizeof(int)*size2);

	for (i = 0; i < size1; i++) {
		L[i] = pData[l + i];
	}
	for (j = 0; j < size2; j++) {
		R[j] = pData[mid + l + j];
	}
	i = 0;
	j = 0;

	while (i < size1 && j < size2) {
		if (L[i] <= R[j]) {
			pData[k] = L[i];
			i++;
		}
		else {
			pData[k] = R[j];
			j++;
		}
		k++;
	}
	while (i < size1) {
		pData[k] = L[i];
		i++;
		k++;
	}
	while (j < size2) {
		pData[k] = R[j];
		j++;
		k++;
	}
	DeAlloc(L);
	DeAlloc(R);
}

// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int* pData, int n)
{
	int i, j, item;
	for (i = 1; i < n; i++) {
		item = pData[i];
		for (j = i - 1; j >= 0; j--) {
			if (pData[j] > item) {
				pData[j + 1] = pData[j];
			}
			else
				break;
		}
		pData[j + 1] = item;
	}
}

// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int* pData, int n)
{
	int i, j, temp;
	for (i = 0; i < n - 1; i++) {
		for (j = 0; j < n - i - 1; j++) {
			if (pData[j] > pData[j + 1]) {
				temp = pData[j];
				pData[j] = pData[j + 1];
				pData[j + 1] = temp;
			}
		}
	}
}

// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int* pData, int n)
{
	int i, j, minimum, temp;
	for (i = 0; i < n - 1; i++) {
		minimum = i;
		for (j = i + 1; j < n; j++) {
			if (pData[j] < pData[minimum]) {
				minimum = j;
			}
		}
		temp = pData[i];
		pData[i] = pData[minimum];
		pData[minimum] = temp;
	}
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)Alloc(sizeof(int) * dataSz);
		// Implement parse data block
		for (int i = 0; i < dataSz; i++) {
			fscanf(inFile,"%d ", &*ppData[i]);
		}

	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};
	
	for (i=0;i<3;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)Alloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Selection Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		selectionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Insertion Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		insertionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Bubble Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		bubbleSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

                printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		DeAlloc(pDataCopy);
		DeAlloc(pDataSrc);
	}
	
}
