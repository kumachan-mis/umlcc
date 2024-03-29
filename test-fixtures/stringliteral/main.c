void put_str(char* s);
void put_blank_line(void);

char global_array[20] = "global char array";
char tricky_global_array[21] = "tricky glo\0bal array";

int main(void) {
    char local_array[19] = "local char array";
    char tricky_local_array[20] = "tricky lo\0cal array";
    char* local_pointer = "local char pointer";
    char* tricky_local_pointer = "local ch\0ar pointer";

    put_str(global_array);
    put_str(local_array);
    put_str(local_pointer);
    put_str("inline string");
    put_blank_line();

    put_str(tricky_global_array);
    put_str(tricky_global_array + 11);
    put_str(tricky_local_array);
    put_str(tricky_local_array + 10);
    put_str(tricky_local_pointer);
    put_str(tricky_local_pointer + 9);
    put_str("tricky inl\0ine string");
    put_str(&"tricky inl\0ine string"[11]);

    return 0;
}
