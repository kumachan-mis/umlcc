int put_str(char* s);

char global_array[20] = "global char array";

int main() {
    char local_array[20] = "local char array";
    char* local_pointer = "local char pointer";

    put_str(global_array);
    put_str(local_array);
    put_str(local_pointer);
    put_str("inline string");

    return 0;
}
