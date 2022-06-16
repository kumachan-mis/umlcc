void put_str(char* s);

char gca[20] = "global char array";
char* gcp = "global char pointer";

void sliteral() {
    char lca[20] = "local char array";
    char* lcp = "local char pointer";

    put_str("inline literal");
}
