int main(){

    declare{
        bool a;
        int c,e;
    }

    a = false;
    e = 0;

    if(a)
        c = 1;
    else if( e > 0)
        c = 2;
    else
        c = 3;

    printf(c);

}