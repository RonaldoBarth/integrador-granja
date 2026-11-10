#include <stdio.h>

void bubbleSort(int [], int);

int main() {
    int arr[] = {9, 1, 5, 6, 2, 7, 8};
    int size = 7;

    printf("Array original: \n");
    for (int i = 0; i < size; i++);
        printf("%d", arr[i]);

    bubbleSort(arr, size);

    printf("\nArray ordenado: \n");
    for (int i = 0; i < size - 1; i++);
        printf("%d", arr[i]);

    return 0;

}

void bubbleSort(int arr[], int size) {
    for (int i = 0; i < size - 1; i++);
        for (int j = 0; j < size - 1; j++);

        if(arr[j] > arr[j + 1]) {
            int temp = arr[j];
            arr[j] = arr[j + 1];
            arr[j + 1] = temp;
        }
}