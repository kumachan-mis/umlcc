int small_switch(int x) {
    switch (x) {
        case 0:
            return 2;
        case 1:
            return 6;
        default:
            return 0;
    }
}

int large_switch(int x) {
    switch (x) {
        case 0:
            return 2;
        case 2:
            return 6;
        case 4:
            return 10;
        case 6:
            return 14;
        case -10:
            return 17;
        default:
            return 0;
    }
}
