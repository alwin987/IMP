#include <stdio.h>

int main() {
    int incoming, outgoing, buck_size, n, store = 0;

    printf("Enter bucket size, outgoing rate, and number of inputs: ");
    scanf("%d %d %d", &buck_size, &outgoing, &n);

    while (n--) {
        printf("\nEnter incoming packet size: ");
        scanf("%d", &incoming);

        if (incoming + store > buck_size) {
            printf("Dropped %d packets\n", (incoming + store) - buck_size);
            store = buck_size;
        } else {
            store += incoming;
        }

        printf("Buffer size: %d/%d\n", store, buck_size);

        store -= outgoing;
        if (store < 0) store = 0;

        printf("After outgoing: %d/%d packets in buffer\n", store, buck_size);
    }

    return 0;
}

