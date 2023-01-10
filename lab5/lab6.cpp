#include <cstdio>
#include <cstdlib>

int main() {
   int left;
   int right;
   int result;
   char op[5];
   while (true) {
       printf("Enter operation +,-,*,%, or / (type q to quit): ");
       scanf("%5s", op);
       if (op[0] == 'q') {
          break;
       }
       printf("Enter left operand: ");
       scanf("%d", &left);
       printf("Enter right operand: ");
       scanf("%d", &right);
       switch (op[0]) {
          case '+':
             result = left + right;
             break;
          case '-':
             result = left - right;
             break;
          case '*':
             result = left * right;
             break;
          case '/':
             result = left / right;
             break;
          case '%':
             result = left % right;
             break;
       }
       printf("%d %c %d = %d\n", left, op[0], right, result);
   }
   exit(0);
}
