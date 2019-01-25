int main () {

  declare{
    int n, curr;
  }
  printf("Introduce numero: ") ;
  scanf(n) ;
  curr = 2 ;
  n = 8;

  while (curr <= n) {

    if ( curr == n ) {

      printf(curr) ;
      curr = curr +1;
    } else {

      curr = curr + 1 ;
    }
  }

  printf("\n") ;
}