#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>


int write_the_char(char c) {
    return write(1, &c, 1);
}

int write_the_str(char *str) {
    
    if (str == NULL) {
        // Handle NULL strings
        return write_the_str("(null)");
    }
    int len = 0;
    while (*str != '\0') {
        if (write_the_char(*str) < 0)
            return -1;
        len++;
        str++;
    }
    return len;
}

int write_the_int(int n) {
    int len = 0;
    if (n < 0) {
        if (write_the_char('-') < 0)
            return -1;
        len++;
        n = -n;
    }
    if (n / 10 != 0)
        len += write_the_int(n / 10);
    if (write_the_char('0' + n % 10) < 0)
        return -1;
    return len + 1;
}

int write_the_float(double f) {
    int len = 0;
    int int_part = (int)f;
    double frac_part = f - int_part;
    len += write_the_int(int_part);
    if (frac_part != 0) {
        if (write_the_char('.') < 0)
            return -1;
        len++;
        frac_part *= 1000000; // 6 decimal places
        len += write_the_int((int)frac_part);
    }
    return len;
}

int write_the_octal(unsigned int n) {
    int len = 0;
    if (n / 8 != 0)
        len += write_the_octal(n / 8);
    if (write_the_char('0' + n % 8) < 0)
        return -1;
    return len + 1;
}


int write_the_hex(uintptr_t n, int uppercase) {
    const char *hex_digits = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";
    int len = 0;
    if (n / 16 != 0)
        len += write_the_hex(n / 16, uppercase);
    if (write_the_char(hex_digits[n % 16]) < 0)
        return -1;
    return len + 1;
}


int write_the_ptr(void *p) {
    // Cast pointer to unsigned long to ensure correct size
    unsigned long ptr_val = (unsigned long)p;
    // Output pointer value in hexadecimal
    return write_the_hex(ptr_val, 1);
}

int my_printf(char * restrict format, ...) {
    va_list args;
    va_start(args, format);

    int len = 0;
    char *str;
    int n;
    unsigned int u;
    double f;
    char c;
    void *p;

    while (*format != '\0') {
        if (*format == '%') {
            format++;
            switch (*format) {
                case 's':
                    str = va_arg(args, char *);
                    len += write_the_str(str);
                    break;
                case 'd':
                    n = va_arg(args, int);
                    len += write_the_int(n);
                    break;
                case 'u':
                    u = va_arg(args, unsigned int);
                    len += write_the_int(u);
                    break;
                case 'o':
                    u = va_arg(args, unsigned int);
                    len += write_the_octal(u);
                    break;
                 case 'x':
                    u = va_arg(args, uintptr_t);
                    len += write_the_hex(u, 1); // Use lowercase for hex digits
                    break;

                case 'X':
                    u = va_arg(args, uintptr_t);
                    len += write_the_hex(u, 0); // Use uppercase for hex digits
                    break;
                case 'f':
                    f = va_arg(args, double);
                    len += write_the_float(f);
                    break;
                case 'c':
                    c = (char)va_arg(args, int);
                    if (write_the_char(c) < 0)
                        return -1;
                    len++;
                    break;
                case 'p':
                    p = va_arg(args, void *);
                    if (p == NULL) {
                        len += write_the_str("(nil)");
                    } else {
                        // Output the pointer value in lowercase hexadecimal
                        len += write_the_str("0x");
                        len += write_the_hex((uintptr_t)p, 0); // Use lowercase for pointer addresses
                    }
                    break;

                default:
                    if (write_the_char('%') < 0)
                        return -1;
                    len++;
                    if (*format != '\0') {
                        if (write_the_char(*format) < 0)
                            return -1;
                        len++;
                    }
                    break;
            }
        } else {
            if (write_the_char(*format) < 0)
                return -1;
            len++;
        }
        format++;
    }

    va_end(args);
    return len;
}



int main(){
    int num = 10;
    float num2 = 2.342;
    char ch = 'd';
    char str[] = "Gombawa";

    my_printf("%d is an integer type number\n", num);
    my_printf("%f is a double type number\n", num2);
    my_printf("%c is a char type\n", ch);
    my_printf("Hello %s! The is my printf result\n", str);

    return 0;

}
