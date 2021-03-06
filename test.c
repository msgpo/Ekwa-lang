#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

extern void _debug_EMPTYBUFF(void);

extern uint32_t _runtime_IFNE(char *, char *);
extern uint32_t _runtime_IFE(char *, char *);
extern uint32_t _runtime_IFS(char *, char *);
extern uint32_t _runtime_IFB(char *, char *);
extern uint32_t _runtime_ALEN(char *);
extern void _runtime_AADD(char *);
extern void _runtime_CAT(char *, char *);
extern void _runtime_VAL(char *, char *);
extern void _runtime_ADD(char *, char *);
extern void _runtime_PBUF(char *);
extern void _runtime_BUFF(char *);
extern void _runtime_WRT(char *);
extern void _runtime_SHOW(char *);
extern char *_runtime_VAR(char);

extern void _runtime_ARM(char *, char *);
extern void _runtime_ARW(char *, char *);
extern void _runtime_ABUF(char *, char *);

extern void _runtime_I2F(char *);
extern void _runtime_F2I(char *);

void print_hex(char *buff, size_t len) {
	for (size_t i = 0; i < len; i++) {
		printf("0x%x ", buff[i]);
	}

	printf("\n");
}

void test_1(void) {
	// New var
	char *var1 = _runtime_VAR(0), *var2;
	printf("New var: %p & Type: %d\n", var1, var1[0]);

	// Set var value
	char *hl = "Hello world!", *content, *d;
	uint32_t size = strlen(hl) + 1, vsize;

	content = (char *)malloc(size + 4);
	memcpy(content + 4, hl, size);
	memcpy(content, &size, 4);

	_runtime_VAL(var1, content);

	memcpy(&vsize, var1 + 1, 4);
	memcpy(&d, var1 + 5, sizeof(char *));
	printf("Var size: %d, content: %s, ptr content:"
		" %p\n\n", vsize, d, d);

	// Set buffer value and write to var (as pointer)
	var2 = _runtime_VAR(0);
	_runtime_PBUF(var1);
	_runtime_WRT(var2);

	memcpy(&vsize, var2 + 1, 4);
	memcpy(&d, var2 + 5, sizeof(char *));
	printf("> From buffer:\nVar size: %d, content: %s, "
		"ptr content: %p\n\n", vsize, d, d);

	// Set buffer value and write to var (as new pointer)
	_runtime_BUFF(var1);
	_runtime_WRT(var2);

	memcpy(&vsize, var2 + 1, 4);
	memcpy(&d, var2 + 5, sizeof(char *));
	printf("> From buffer:\nVar size: %d, content: %s, "
		"ptr content: %p\n\n", vsize, d, d);

	// Vars comparing (are equal)
	uint32_t res = _runtime_IFE(var1, var2);
	printf("Comparing result: %d\n", res);

	// Vars comparing (aren't equal)
	res = _runtime_IFNE(var1, var2);
	printf("Comparing result (IFE): %d\n", res);

	// New integer vars with comparing
	char *var_num1 = _runtime_VAR(3);
	char *var_num2 = _runtime_VAR(3);

	free(content);
	size = 4;
	content = (char *)malloc(4 + size);

	memcpy(content + 4, &(int){10}, size);
	memcpy(content, &size, 4);
	_runtime_VAL(var_num1, content);

	memcpy(content + 4, &(int){50}, size);
	_runtime_VAL(var_num2, content);

	int *va1, *va2;

	memcpy(&va1, var_num1 + 5, 4);
	memcpy(&va2, var_num2 + 5, 4);

	printf("Var addresses: %p : %p\nVar val: %d : %d\n\n",
		var_num1, var_num2, *va1, *va2);

	res = _runtime_IFS(var_num1, var_num2);
	printf("Comparing result (IFS): %d\n", res);

	res = _runtime_IFB(var_num1, var_num2);
	printf("Comparing result (IFB): %d\n", res);

	// New float vars with comparing
	char *var_flt1 = _runtime_VAR(4);
	char *var_flt2 = _runtime_VAR(4);
	float a1 = 7.0, a2 = 4.2;

	free(content);
	size = sizeof(float);
	content = (char *)malloc(4 + size);


	memcpy(content + 4, &a1, size);
	memcpy(content, &size, 4);
	_runtime_VAL(var_flt1, content);

	memcpy(content + 4, &a2, size);
	_runtime_VAL(var_flt2, content);

	res = _runtime_IFS(var_flt1, var_flt2);
	printf("Comparing result (IFS): %d\n\n", res);

	// Strings concatenation
	char *var_str1 = _runtime_VAR(0);
	char *var_tmp = _runtime_VAR(0);
	size = strlen(hl);
	free(content);

	content = (char *)malloc(size + 4);
	memcpy(content + 4, hl, strlen(hl));
	memcpy(content, &size, 4);

	_runtime_VAL(var_str1, content);
	_runtime_CAT(var_str1, var1);
	_runtime_WRT(var_tmp);

	memcpy(&d, var_tmp + 5, sizeof(char *));
	size = (uint32_t)*(var_tmp + 1);
	printf("Concat: %s, size: %d\n\n", d, size);

	// Add to number (int)
	_runtime_ADD(var_num1, var_num2);
	_runtime_WRT(var_num1);
	memcpy(&d, var_num1 + 5, 4);
	printf("Add res: %d\n", (int)*d);

	// Add to number (float)
	_runtime_ADD(var_flt1, var_flt2);
	_runtime_WRT(var_flt1);

	memcpy(&d, var_flt1 + 5, 4);
	memcpy(&a1, d, 4);
	printf("Fadd res: %f\n\n", a1);

	// Show string
	printf("Show string: \n");
	_runtime_SHOW(var_tmp);
	printf("\n");
}

void test_2() {
	char *array = _runtime_VAR(2), *var1 = _runtime_VAR(0),
		*var2 = _runtime_VAR(0), *var_num;

	char *hl1 = "Hello world!", *hl2 = "Aloha!", *content, *content2,
		*d, *d2, *el1, *el2;
	uint32_t size = strlen(hl1) + 1;

	content = (char *)malloc(size + 4);
	memcpy(content + 4, hl1, size);
	memcpy(content, &size, 4);

	size = strlen(hl2) + 1;
	content2 = (char *)malloc(size + 4);
	memcpy(content2 + 4, hl2, size);
	memcpy(content2, &size, 4);

	_runtime_VAL(var1, content);
	_runtime_VAL(var2, content2);
	_runtime_BUFF(var1);
	_runtime_AADD(array);
	_runtime_BUFF(var2);
	_runtime_AADD(array);
	_runtime_BUFF(var1);
	_runtime_AADD(array);

	memcpy(&d, array + 5, 4);
	printf("%p\n", d);
	memcpy(&d2, d, 4);
	printf("%p\n", d);
	memcpy(&el1, d2 + 5, 4);
	printf("El1: %s\n", el1);

	memcpy(&d2, d + 4, 4);
	printf("%p\n", d);
	memcpy(&el2, d2 + 5, 4);
	printf("El2: %s\n", el2);
	free(content);
	free(content2);


	content = (char *)malloc(8);
	memcpy(content + 4, &(int){1}, 4);
	memcpy(content, &(int){4}, 4);

	// Second element removing.
	var_num = _runtime_VAR(3);
	_runtime_VAL(var_num, content);
	_runtime_ARM(array, var_num);

	size = _runtime_ALEN(array);
	printf("Current size after removing: %d\n", size);

	memcpy(&d, array + 5, 4);
	printf("%p\n", d);
	memcpy(&d2, d, 4);
	printf("%p\n", d);
	memcpy(&el1, d2 + 5, 4);
	printf("El1: %s\n", el1);

	memcpy(&d2, d + 4, 4);
	printf("%p\n", d);
	memcpy(&el2, d2 + 5, 4);
	printf("El2: %s\n", el2);
	free(content);

	// Element rewrite.
	char *var_str = _runtime_VAR(0), *str2 = "Test test 2 test.";
	size = strlen(str2) + 1;

	content = (char *)malloc(size + 4);
	memcpy(content + 4, str2, size);
	memcpy(content, &size, 4);

	_runtime_VAL(var_str, content);
	_runtime_BUFF(var_str);
	_runtime_ARW(array, var_num);

	memcpy(&d, array + 5, 4);
	printf("%p\n", d);
	memcpy(&d2, d, 4);
	printf("%p\n", d);
	memcpy(&el1, d2 + 5, 4);
	printf("El1: %s\n", el1);

	memcpy(&d2, d + 4, 4);
	printf("%p\n", d);
	memcpy(&el2, d2 + 5, 4);
	printf("El2: %s\n", el2);

	_debug_EMPTYBUFF();

	// Write element to var2.
	_runtime_ABUF(array, var_num);
	_runtime_WRT(var2);

	memcpy(&d, var2 + 5, 4);
	printf("Written: %s\n", d);
}

void test_3(void) {
	char *var_float = _runtime_VAR(4);
	char *var_int = _runtime_VAR(3);
	char *content, *d;
	uint32_t size;
	float tmp = -120.14;
	int tmp_int = -31;

	// Int to float.
	size = sizeof(int);
	content = (char *)malloc(4 + size);

	memcpy(content + 4, &tmp_int, 4);
	memcpy(content, &size, 4);

	tmp_int = 0;

	_runtime_VAL(var_int, content);

	memcpy(&d, var_int + 5, 4);
	memcpy(&tmp_int, d, 4);
	printf("Int num: %d\n", tmp_int);

	_runtime_I2F(var_int);
	_runtime_WRT(var_float);

	memcpy(&d, var_float + 5, 4);
	memcpy(&size, var_float + 1, 4);
	memcpy(&tmp, d, 4);

	printf("Converted number: %f, size: %d\n", tmp, size);
	free(content);

	// Float to int.
	tmp = -120.14;
	size = sizeof(float);
	content = (char *)malloc(4 + size);

	memcpy(content + 4, &tmp, 4);
	memcpy(content, &size, 4);

	_runtime_VAL(var_float, content);
	_runtime_F2I(var_float);
	_runtime_WRT(var_int);

	memcpy(&d, var_int + 5, 4);
	memcpy(&tmp_int, d, 4);

	printf("Converted number: %d\n", tmp_int);
}

int main() {
	//test_1();
	test_2();
	test_3();
}
