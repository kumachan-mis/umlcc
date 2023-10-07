void* malloc(unsigned int size);
void free(void* ptr);

void put_int(int x);

void merge_sort(int* array, int left, int right) {
    if (left >= right) {
        return;
    }

    int mid = (left + right) / 2;
    merge_sort(array, left, mid);
    merge_sort(array, mid + 1, right);

    int i = left;
    int j = mid + 1;
    int k = 0;
    int* temp = malloc(sizeof(int) * (right - left + 1));
    while (i <= mid && j <= right) {
        if (array[i] < array[j]) {
            temp[k] = array[i];
            ++i;
        } else {
            temp[k] = array[j];
            ++j;
        }
        ++k;
    }

    while (i <= mid) {
        temp[k] = array[i];
        ++i;
        ++k;
    }

    while (j <= right) {
        temp[k] = array[j];
        ++j;
        ++k;
    }

    i = left;
    k = 0;
    while (i <= right) {
        array[i] = temp[k];
        ++i;
        ++k;
    }

    free(temp);
}

int main(void) {
    int array[10] = {13, 3, 9, 8, 5, 1, 4, 11, 2, 7};
    merge_sort(array, 0, 9);

    int i = 0;
    while (i < 10) {
        put_int(array[i]);
        i++;
    }

    return 0;
}
