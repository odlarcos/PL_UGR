int main () {

  declare {
    int ve ;
    double vf ;
    char vc ;
    bool vl ;
  }

  int funcionA (int a1, double a2, char a3) {

    declare {
      int x1, x2 ;
    }

    char funcionB (char b1, bool b2) {

      declare {
        double xf, x2 ;
      }
      
      double funcionC (bool c1, int c2) {

        declare {
          double x1 ;
        }

        x1 = 1.3 ;
        if (c2 > 10) {
          c2 = c2 - 1 ;
        } else {
          x1 = 3.1 ;
        }
        return x1 ;
      }

      xf = funcionC (true, 10) ;
      x2 = xf * (funcionC(false, 1) - funcionC(true, 23)) / 10.0 ;

      while (x2 * funcionC(false, 1) - xf < 10.0) {
        x2 = x2 * xf ;
      }
    }
  }

  double funcionD (double d1) {

    declare {
      int dato ;
      int valor ;
    }

    char funcionE (char e1, char e2) {

      printf("Introduzca dos caracteres: ") ;
      scanf(e1) ;
      scanf(e2) ;

      if (e1 == 'a') {
        return e1 ;
      } else if (e1 == 'b') {
        return e2 ;
      } else {
        return ' ' ;
      }

    }

    if (d1 > 0.0) {

      dato = 2 ;
      dato = valor * 20 / dato ;
    
    } else {
      valor = valor * 100 ;
      d1 = d1 / 1000.0 ;
    }

    return d1 ;
  }
}
