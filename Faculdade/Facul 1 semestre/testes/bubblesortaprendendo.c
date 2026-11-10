#include <stdio.h>

void bubbleSort(int [], int);

int main() {
    int arr[] = {5, 2, 7, 1, 0, 9, 10, 5, -1, 8};
    int size = 10;

    printf("\n\nArrar original: ");
    for (int i = 0; i < size; i++)
        printf("%d", arr[i]);

    bubbleSort(arr, size);

    printf("\n\nArray ordenado: ");
    for (int i = 0; i < size; i++)
        printf("%d", arr[i]);

    return 0;
}

void bubbleSort(int arr[], int size) {
    for (int i = 0; i < size - 1; i++)
        for (int j = 0; j < size - i - 1; j++)

        if(arr[j] > arr[j + 1]) {
            int temp = arr[j];
            arr[j] = arr[j + 1];
            arr[j + 1] = temp;
        }
}