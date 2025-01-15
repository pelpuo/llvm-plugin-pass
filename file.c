int main() {
    volatile int x = 4; // Example input
    int result = 0;

    // Simple loop with conditionals
    for (int i = 1; i <= 5; i++) {
        if (i % 2 == 0) {
            result += i;
        } else {
            result -= i;
        }
    }

    // Switch-case with nested conditions
    switch (x) {
        case 2:
            result *= 2;
            break;
        case 4:
            if (result % 3 == 0) {
                result += 10;
            } else {
                result -= 5;
            }
            break;
        default:
            result = -1;
            break;
    }

    // Conditional return
    return (result > 0) ? 0 : 1;
}
