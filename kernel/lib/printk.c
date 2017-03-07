#include "lib/common.h"
void vfprintf(void (*printer)(char), const char *ctl, void **args) {
	int num_temp;
	char *str_temp;
	char char_temp;
	bool pre_zero;
	bool isNeg;
	int i;
	const char *str = ctl;
	for(;*str != '\0'; str ++) {
		if(*str == '%') {
			str++;
			switch(*str){
			case 'd':
				num_temp = (int) *args;
				isNeg = false;
				if((num_temp >> 31) & 0x1) {
					printer('-');
					num_temp = ~num_temp;
					isNeg = true;
				}
				pre_zero = true;
				i = 1000000000;
				char_temp = num_temp / i;
				while(i != 0) {
					if(char_temp != 0 || pre_zero == false || i == 1) {
						pre_zero = false;
						if(i == 1 && isNeg)
							char_temp++;
						if(char_temp >= 10) {
							printer('a' + char_temp - 10);
						}else{
							printer('0' + char_temp);
						}
					}
					num_temp = num_temp % i;
					i = i / 10;
					if(i == 0)
						break;
					char_temp = num_temp / i;
				}
				args++;
				break;
			case 'x':
				num_temp = (int) *args;
				pre_zero = true;
				for(i = 7; i >= 0; i--) {
					char_temp = (num_temp >> (i * 4)) & 0xf;
					if(pre_zero == true && char_temp == 0 && i != 0) {
						pre_zero = true;
						continue;
					}else{
						pre_zero = false;
					}
					if(char_temp >= 10) {
						printer('a' + char_temp - 10);
					}else{
						printer('0' + char_temp);
					}
				}
				args++;
				break;
			case 's':
				str_temp = (char *) *args;
				while(*str_temp != '\0'){
					printer(*str_temp);
					str_temp++;
				}
				args++;
				break;
			case 'c':
				char_temp = (int) *args;
				printer(char_temp);
				args++;
				break;
			default: str--;
			}
		}else{
			printer(*str);
		}
	}
}

extern void serial_printc(char);

/* __attribute__((__noinline__))  here is to disable inlining for this function to avoid some optimization problems for gcc 4.7 */
void __attribute__((__noinline__)) 
printk(const char *ctl, ...) {
	void **args = (void **)&ctl + 1;
	vfprintf(serial_printc, ctl, args);
}
