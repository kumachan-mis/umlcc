void put_int(int x);

void quick_sort(int* array, int begin, int end) {
    if (begin >= end) {
        return;
    }

    int i = begin;
    int j = end;
    int pivot = array[begin];

    while (i < j) {
        while (i < j && array[j] > pivot) {
            j--;
        }
        array[i] = array[j];
        while (i < j && array[i] <= pivot) {
            i++;
        }
        array[j] = array[i];
    }

    array[i] = pivot;

    quick_sort(array, begin, i - 1);
    quick_sort(array, j + 1, end);
}

int main(void) {
    int array[13] = {13, 3, 9, 8, 5, 1, 4, 11, 2, 7, 2, 4, 6};
    quick_sort(array, 0, 12);

    for (int i = 0; i < 13; ++i) {
        put_int(array[i]);
    }

    return 0;
}
