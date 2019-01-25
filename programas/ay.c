int main ( ) {

  declare{
    int ay;
    char oo;
    int b;
    int a;
  }

  int f1(int a, int b){
    declare{
      int x;
      char y;
    }

    int f12(int a, int b){
      return b+a;
    }

    if(b < a)
      return a;

    return f12(b,a);
  }

  a = 2;
  b = 5;
  printf(a);
  printf("\n");

  a = (2*8) + 9/5;
  b = f1(b,a);
  if( b == 7 )
    printf(b);

  scanf(b);

  while( b > 0){
    printf(b);
    b = b-1;
  }
}
