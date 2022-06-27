#include <iostream>
#include <string>
#include <cstdio>
#include <ctype.h>
#include <cstdlib>
#include <vector>
#include <stack>
#include <algorithm>
 
using namespace std;
 
enum type_of_lex {
    LEX_NULL,  LEX_STRUCT,                                                                                               /* 1*/
    LEX_FOR,LEX_GOTO,LEX_BREAK, LEX_AND, LEX_BOOL, LEX_DO, LEX_ELSE, LEX_IF, LEX_FALSE, LEX_INT,                         /*11*/
    LEX_STR,                                                                                                             /*12*/
    LEX_NOT, LEX_OR, LEX_PROGRAM, LEX_READ, LEX_THEN, LEX_TRUE, LEX_WHILE, LEX_WRITE,                                    /*20*/
    LEX_FIN,                                                                                                             /*21*/
    LEX_POINT,LEX_EQ_DOUB, LEX_FIG_OPEN, LEX_FIG_CL,
    LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_LPAREN, LEX_RPAREN, LEX_EQ, LEX_LSS,                                        /*32*/
    LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ, LEX_UPQUOT, LEX_DOUB_QUOT,            /*43*/
    LEX_NUM,                                                                                                             /*43*/
    LEX_ID,                                                                                                              /*44*/
    POLIZ_LABEL,                                                                                                         /*45*/
    POLIZ_ADDRESS_STRUCT,
    POLIZ_ADDRESS,                                                                                                       /*46*/
    POLIZ_GO,                                                                                                            /*47*/
    POLIZ_FGO,                                                                                                           /*48*/
    LEX_LAB,                                                                                                             
    LEX_UN_MINES
};
 
 
class Lex {
    type_of_lex t_lex;  //класс лексемы
    int v_lex = 0;          //номер строки в таблице лексем TID
    int f_field = -1;        //номер поля в таблице структур
public:
    Lex ( type_of_lex t = LEX_NULL, int v = 0 ): t_lex (t), v_lex (v), f_field(0) { }
    type_of_lex  get_type () const { return t_lex; }
    int get_value () const { return v_lex; }
    int get_field () const { return f_field; }
    friend ostream & operator<< ( ostream &s, Lex l );
  /*friend ostream & operator<< ( ostream &s, Lex l ){
        s << '(' << l.t_lex << ',' << l.v_lex << ");";
        return s;
    }*/

    Lex ( type_of_lex t, int v, int f ): t_lex (t), v_lex (v), f_field (f) { }
};
 
 
class Ident {
    string name;      //имя переменной
    bool declare;     //описан в программе или нет
    type_of_lex type; //тип переменной
    bool assign;      //было ли присвоение или нет
    int value;        //само значение переменной, , если это int/bool
    string str;       //само значение переменной, если это строка
    int lab;          //для меток
    int type_of_struct; //для структур
public:
    Ident() { 
        declare = false; 
        assign  = false;
        lab = -1; 
    }
    bool operator== ( const string& s ) const { 
        return name == s; 
    }
    Ident ( const string n ) {
        name = n;
        declare = false; 
        assign = false;
    }
    string get_name () const { return name; }
    bool get_declare () const { return declare; }
    void put_declare () { declare = true; }
    type_of_lex get_type () const { return type; }
    void put_type ( type_of_lex t ) { type = t; }
    bool get_assign () const {return assign; }
    void put_assign () { assign = true; }
    int  get_value () const {return value; }
    void put_value ( int v ) { value = v;}
    void put_lab(int l) {lab = l;}
    int get_lab() {return lab;}
    string  get_str () const {return str; }
    void put_str ( string s ) { str = s;}
    void put_struct (int i) { type_of_struct = i;}
    int get_type_of_struct() { return type_of_struct;} 
};
 
vector<Ident> TID;  //global хранение индентификаторов имён переменных

int put ( const string & buf ) {
    vector<Ident>::iterator k;
 
    if ( ( k = find(TID.begin(), TID.end(), buf ) ) != TID.end () )
        return k - TID.begin();   //номер позиции с нуля
    TID.push_back ( Ident(buf) );
    return TID.size () - 1;      //номер позиции с нуля
}
 
class areas{ //класс структур
    string name; //имя структуры
public:
    vector<Ident> field; //поля
    areas();
    areas(const string & buf): name(buf){}
    string get_name () const { return name; }
    
};

vector<areas> my_struct; //вектор описания структур
int i_str = 0;

vector<areas> code_struct; //вектор переменных типа структур, их поля

void add_field(int  c_val){
    my_struct[i_str].field.push_back( TID[c_val]);
}

    
void put_struct(const string & buf){
    my_struct.push_back ( areas(buf) );
}

void put_struct(int k){ //k - номер в тамблице my_struct
    code_struct.push_back(areas("1"));
    for (int i = 0; i < my_struct[k].field.size(); i++){
        code_struct[code_struct.size()-1].field.push_back(my_struct[k].field[i]);

    }
} 
 
class Scanner {
    FILE * fp;
    char   c;
    int look ( const string buf, const char ** list ) { //поиск buf в list
        int i = 0;
       // cout<<"you are in function look"<<endl;
        //cout<<buf<<endl;
        while ( list[i] ) {
            //cout<<buf<<endl;
            //cout<<list[i - 1]<<endl;
           // cout<<buf == list[i-1]
            if ( buf == list[i] )
                return i;
            ++i;
        }
       //cout<<"i = "<<i<<endl;
        return 0;
    }
    void gc () {
        c = fgetc (fp);  //считать очередной символ
    }
public:
    static const char * TW [], * TD [];  //таблица служебных слов, таблица ограничителей
    Scanner ( const char * program ) {
        if ( !(fp = fopen ( program, "r" )) ) 
            throw  "can’t open file" ;
    }
    Lex get_lex ();
};

const char *
Scanner::TW    [] = { "","struct","for","goto","break", "and", "bool", "do", "else", "if", "false", "int","string", "not", "or", "program",
                      "read", "then", "true", "while", "write", NULL }; //17+NULL
 
 
const char *
Scanner::TD    [] = {"",".","==","{","}" , ";", ",", ":", "(", ")", "=", "<", ">", "+", "-", "*", "/", "<=", "!=", ">=","'","\"", NULL }; //21+NULL

 
Lex Scanner::get_lex () {
    enum    state { H, IDENT, NUMB, COM_DET,  COM_DET_F, COM, ALE, NEQ };
    int     d, j;
    string  buf;
    state   CS = H;
    do {
        gc ();
        switch ( CS ) {
            case H:
            //cout<<"you are in state H"<<endl;
                if ( c==' ' || c == '\n' || c== '\r' || c == '\t' );  
                    else if ( isalpha (c) ) {   //является ли буквой
                        buf.push_back (c);
                        CS  = IDENT;
                    }
                    else if ( isdigit (c) ) {
                        d   = c - '0';
                        CS  = NUMB;
                    } 
                    else if ( c== '/' ) { 
                        CS  = COM_DET;
                    }
                    else if ( c == '=' || c == '<' || c == '>' ) { 
                        buf.push_back (c);
                        CS  = ALE; 
                    }
                    else if (c == EOF) {
                       // cout<<"END of file!"<<endl;
                        return Lex ( LEX_FIN );
                    }

                    else if (c == '!') {
                        buf.push_back (c);
                        CS  = NEQ;
                    }
                    else {   //+, -
                        //cout<<"you are in state +/-"<<endl;
                        buf.push_back (c);
                        cout<<endl<<c<<" ";
                        if ( ( j = look ( buf, TD) ) ){
                          //  cout<<endl<<"j = "<<j<<" ";
                            return Lex ( (type_of_lex)( j + (int) LEX_FIN ), j );
                        }
                        else
                            throw c;
                    }
                    break;
            case IDENT:
                if ( isalpha (c) || isdigit (c) ) {
                    buf.push_back (c); 
                }
                else {
                    ungetc ( c, fp );  //следующая лексема
                    if ( (j = look ( buf, TW) ) ) {
                        cout<<endl<<buf<<" ";
                        return Lex ( (type_of_lex) j, j );
                        }
                        else {
                            cout<<" ";
                        j   = put ( buf );
                        cout<<endl<<buf<<" ";
                        return Lex ( LEX_ID, j );
                    }
                }
                break;
            case NUMB:
                if ( isdigit (c) ) {
                    d = d * 10 + ( c - '0' );
                }
                else {
                    ungetc ( c, fp );
                    cout<<endl<<d<<" ";
                    return Lex ( LEX_NUM, d );
                }
                break;

            case COM_DET:
                if (c == '*') CS = COM;
                else {
                    buf.push_back ('/'); 
                    if ( ( j = look ( buf, TD) ) ){
                        cout<<endl<<c<<" ";
                        return Lex ( (type_of_lex)( j + (int) LEX_FIN ), j );
                    }
                    buf.push_back (c);
                    ungetc ( c, fp );
                    CS = H; 
                }
                break;

            case COM:
           // cout<<"you are in comment"<<endl;
                if ( c == '*' ) {
                    CS  = COM_DET_F;
                }
                else if ( c == EOF )
                    throw c;
                break;

            case COM_DET_F:
                if (c == '/') CS = H;
                else CS = COM;
                break;

            case ALE:
                if ( c == '=' ) {
                    buf.push_back ( c );
                    j   = look ( buf, TD );
                    cout<<endl<<buf<<" ";
                    return Lex ( (type_of_lex) ( j + (int) LEX_FIN ), j );
                }
                else {
                    ungetc ( c, fp );
                    j   = look ( buf, TD );
                    cout<<endl<<buf<<" ";
                    return Lex ( (type_of_lex) ( j + (int) LEX_FIN ), j );
                }
                break;

            case NEQ:
                if ( c == '=' ) {
                    buf.push_back(c);
                    j   = look ( buf, TD );
                    cout<<endl<<buf<<" ";
                    return Lex ( LEX_NEQ, j );
                }
                else
                    throw '!';
                break;
    } //end switch
  } while (true);
}


ostream & operator<< ( ostream &s, Lex l ) {
    string t;
    if ( l.t_lex <= LEX_WRITE )
        t = Scanner::TW[l.t_lex];
    else if ( l.t_lex >= LEX_FIN && l.t_lex <= LEX_DOUB_QUOT )
        t = Scanner::TD[ l.t_lex - LEX_FIN ];
    else if ( l.t_lex == LEX_NUM )
        t = "NUM";
    else if ( l.t_lex == LEX_ID )
        t = TID[l.v_lex].get_name ();
    else if ( l.t_lex == POLIZ_LABEL )
        t = "Label";
    else if ( l.t_lex == POLIZ_ADDRESS )
        t = "Addr";
    else if ( l.t_lex == POLIZ_GO )
        t = "!";
    else if ( l.t_lex == POLIZ_FGO ) 
        t = "!F";
    else if ( l.t_lex == LEX_LAB) 
        t = "LAB";
    else if ( l.t_lex ==  LEX_UN_MINES) 
        t = "@";
    else if (l.t_lex == LEX_STRUCT)
        t = "struct";
    else if (l.t_lex == POLIZ_ADDRESS_STRUCT)
        t = "addr struct";
    else
        throw l;
    s << '(' << t << ',' << l.v_lex << ");" << endl;
    return s;
} 
 
//////////////////////////  Класс Parser  /////////////////////////////////
template <class T, class T_EL>
void from_st ( T & st, T_EL & i ) {
    i = st.top(); st.pop();
}
 
class Parser {
    Lex          curr_lex; //current lexem
    type_of_lex  c_type;   
    int          c_val;    //номер строки в таблице лексем
    Scanner      scan;
    stack < int >           st_int;
    stack < type_of_lex >   st_lex;
    type_of_lex var_type;
    int          c_val_cur;    //номер строки в таблице лексем
    int          c_val_f = -1;
    stack < int >           st_lab; //для меток
    stack < int >           st_loop;

    void  Progr();
    void  Descr1();
    void  Descr2();
    void  Type();
    void  Var();
    void  Const();
    void  Int_const();
    void  Str_const();
    void  Bool_const();
    void  dec ( type_of_lex type);
    void  Ops();
    void  Op();
    void  expr();
    void  E();
    void  T();
    void  F();
    void  I();
    void  func_if();
    void  func_for();
    void  func_while();
    void  check_id ();
    void check_id_struct();
    void  check_op_3 ();
    void  check_op_1();
    void  check_not ();
    void  eq_type ();
    void  check_id_in_read ();
    void  label();
    void  dec_l();
    friend void add_field( int c_val);

    void P();
    void Descr_struct();
    void Descr_str2();
    void Var_struct();
    void dec_field();
    void dec_str();
    void Struct_dec1();
    void Struct_dec2();

    void  get_lex () {
        curr_lex  = scan.get_lex ();
        c_type    = curr_lex.get_type ();
        c_val     = curr_lex.get_value (); 
    }
public:
    vector <Lex> poliz;
    Parser ( const char *program ) : scan (program) { }
    void  analyze();
};
 
void Parser::analyze () {
    get_lex ();
    P ();
    if (c_type != LEX_FIN)
        throw curr_lex;
    int i = 1;
    for_each( poliz.begin(), poliz.end(), [](Lex l){ cout << l; });
    cout<<endl<<"POLIZ"<<endl;
    for ( Lex l : poliz ) {
        cout<<" "<<i<<" " << l;
        i++;
    }
    cout << endl << "Yes!!!" << endl;
}
 
 int k_field = 0; //счетчик полей
void Parser::P(){
    while (c_type == LEX_STRUCT){
        get_lex();
        if (c_type != LEX_ID) throw curr_lex;
        put_struct(TID[c_val].get_name()); //заполнить имя структуры;
        dec_str();  //проверка повторной инициализации структуры
        TID.erase(TID.begin() + c_val);
         cout<<TID[c_val].get_name()<<"ERASE";
        get_lex();
        if (c_type != LEX_FIG_OPEN) throw curr_lex;
        get_lex();
       // cout<<"You are here 1"<<endl;
        Descr_struct(); //заполнить поля структуры
        if (c_type != LEX_FIG_CL) throw curr_lex;
        get_lex();
        if (c_type != LEX_SEMICOLON) throw curr_lex;
        get_lex();
        i_str++;
        k_field = 0; //счетчик полей
    }
    Progr();
}
void Parser::Descr_struct () { //описаниЯ
    while ((c_type == LEX_INT) || (c_type == LEX_STR)|| (c_type == LEX_BOOL)) {
        //cout<<endl<<"Descr_struct"<<endl;
        var_type = c_type;
        //cout<<"You are here 2"<<endl;
        get_lex();
        //cout<<"You are here 3"<<endl;
        Descr_str2();
        if (c_type != LEX_SEMICOLON){
            throw curr_lex;
        }
       // cout<<"!"<<endl;
        get_lex();
    } 
}

void Parser::Descr_str2 () { //описаниЕ
    //cout<<endl<<"Descr_str2"<<endl;
    //get_lex();
    Var_struct();
    dec_field (); //проверка повторной инициализации поля структуры
    get_lex();
    while (c_type == LEX_COMMA){
        get_lex();
        Var_struct();
    }
}

void Parser::Var_struct (){
   // cout<<endl<<"you are in Var"<<endl;
    if (c_type != LEX_ID) throw curr_lex;
    else{
        TID[c_val].put_type(var_type);
        add_field( c_val);
        TID.erase(TID.begin() + c_val);
         cout<<TID[c_val].get_name()<<"ERASE";
        k_field++;
    }
}

void Parser::dec_field () {
    int i = 0;
    while ( i < k_field - 1) {
        //cout <<"k_field = "<<k_field<<endl;
         if (my_struct[i_str].field[i].get_name() == my_struct[i_str].field[k_field-1].get_name() )
            throw "twice initialization";
        i++;
    }
}

void Parser::dec_str () {
    int i = 0;
    while ( i < i_str) {
        if (my_struct[i].get_name() == my_struct[i_str].get_name())
            throw "twice initialization";
        i++;
    }
}

void Parser::Progr () {
    cout<<endl<<"you are in Progr"<<endl;
    if ( c_type == LEX_PROGRAM ) {
        get_lex ();
        if (c_type == LEX_FIG_OPEN){
            get_lex();
            Descr1();
            Ops();
        }
        else throw curr_lex;
        if (c_type != LEX_FIG_CL) {
            //cout<<"Error after Ops()"<<endl;
            throw curr_lex;
        }
       // cout<<"In PROgr ok"<<endl;
    }
    else 
         { cout<<"Error in Progr()"<<endl;
            throw curr_lex;  
        }
    get_lex();
    int i = 0;
    while ( !st_lab.empty () ) {
         from_st ( st_lab, i );
        // cout<<i<<" "<<endl;
        // cout<<TID[i].get_name();
         if (!TID[i].get_declare()) throw "error in labels";
    }
}
 
void Parser::Descr1 () { //описаниЯ
    while ((c_type == LEX_INT) || (c_type == LEX_STR)|| (c_type == LEX_BOOL) || (c_type == LEX_STRUCT)) {
        //cout<<endl<<"Descr1"<<endl;
        Descr2();
        if (c_type != LEX_SEMICOLON){
            throw curr_lex;
        }
       // cout<<"!"<<endl;
        cout<<"var_type = "<<var_type<<endl;
        dec ( var_type );
        get_lex();
    } 
}
 
void Parser::Descr2 () { //описаниЕ
    //cout<<endl<<"Descr2"<<endl;
    Type();
    if (c_type == LEX_STRUCT){
        Struct_dec1();
        while (c_type == LEX_COMMA){
            get_lex();
           // Struct_dec2();
        }
    }
    else{
       // Type();
        get_lex();
        Var();
        while (c_type == LEX_COMMA){
            get_lex();
            Var();
        }
    }
}

void Parser:: Struct_dec1(){
    get_lex();
        if (c_type != LEX_ID) throw curr_lex;
        for (int k = 0; k < my_struct.size(); k++){
            if (my_struct[k].get_name() == TID[c_val].get_name()  ){
                put_struct(k); 
                TID[c_val].put_struct(k);
                break;
            }
        }
        TID.erase(TID.begin() + c_val);
        cout<<TID[c_val].get_name()<<"ERASE";
        get_lex();
        if (c_type != LEX_ID) throw curr_lex;
        st_int.push ( c_val );
        TID[c_val].put_value(code_struct.size() - 1);
        get_lex();
}

void Parser:: Struct_dec2(){
        if (c_type != LEX_ID) throw curr_lex;
        st_int.push ( c_val );
        for (int k = 0; k < my_struct.size(); k++){
           // if (my_struct[k].get_name() == TID[c_val_prev].get_name()  ){
            //    put_struct(k); 
            //    break;
            //}
        }
        TID[c_val].put_value(code_struct.size() - 1);
        get_lex();
}


void Parser::Type () {
  //  cout<<endl<<"you are in Type"<<endl;
    if ((c_type == LEX_INT) || (c_type == LEX_STR)|| (c_type == LEX_BOOL)||(c_type == LEX_STRUCT))
        var_type = c_type;
}

void Parser::Var (){
   // cout<<endl<<"you are in Var"<<endl;
    if (c_type != LEX_ID) throw curr_lex;
    else{
        st_int.push ( c_val );
        int c_val_prev = c_val;
      //  cout<<"you are in 1"<<endl;
        get_lex();
        if (c_type == LEX_EQ){
            poliz.push_back (Lex ( POLIZ_ADDRESS, c_val_prev ) );
        //    cout<<"you are in 2"<<endl;

            get_lex();
            Const();
            poliz.push_back ( Lex ( LEX_EQ) );
            get_lex();
        }
    }
}
 
void Parser::Const(){
   // cout<<endl<<"you are in Const"<<endl;
    if (var_type == LEX_INT) Int_const();
    else if (var_type == LEX_BOOL) Bool_const();
    else if (var_type == LEX_STR) Str_const();
    else throw curr_lex;
}

void Parser:: Int_const(){
    int c_type_prev;
    //cout<<endl<<"you are in Int_const"<<endl;
    if ((c_type == LEX_PLUS) || (c_type == LEX_MINUS)){c_type_prev = c_type; get_lex();}
    if (c_type != LEX_NUM) throw curr_lex;
    poliz.push_back ( curr_lex );
    if (c_type_prev == LEX_MINUS) poliz.push_back ( LEX_UN_MINES );
}

void Parser:: Str_const(){
    //cout<<endl<<"you are in Str_const"<<endl;
    if (c_type != LEX_DOUB_QUOT) throw curr_lex;
    else{
        //cout<<"@";
        get_lex();
        if (c_type != LEX_ID) throw curr_lex;
        poliz.push_back ( Lex(LEX_STR, c_val)  );
        get_lex();
        if (c_type != LEX_DOUB_QUOT) throw curr_lex;
    }
}

void Parser:: Bool_const(){
   // cout<<endl<<"you are in Bool_const"<<endl;
    if ((c_type != LEX_TRUE) && (c_type != LEX_FALSE) )
         {
            //cout<<c_type;
           throw curr_lex;
        }
   // cout<<endl<<"you are in Str_const"<<endl;
        if (c_type == LEX_TRUE) poliz.push_back ( Lex (LEX_TRUE, 1) );
        else poliz.push_back ( Lex (LEX_FALSE, 0) );
}

void Parser::Ops(){
    while(c_type != LEX_FIG_CL){
       // cout<<"you are in ops"<<endl;
        if (c_type == LEX_FIN) throw curr_lex;
        Op();
    }
   // cout<<"you are in ops end"<<endl;
}
int pl = 0;
void Parser::Op(){
    //int pl = 0;
   // cout<<"you are in op"<<endl;
    if (c_type == LEX_IF){
        get_lex();
        func_if();
    }
    else if (c_type == LEX_FOR){
        st_loop.push(1);
        get_lex();
        func_for();
        st_loop.pop();
        if (pl) {
            poliz[pl] = Lex ( POLIZ_LABEL, poliz.size() );
            pl = 0;
        }
    }
    else if (c_type == LEX_WHILE){
        st_loop.push(1);
        get_lex();
        func_while();
        st_loop.pop();
        if (pl) {
            poliz[pl] = Lex ( POLIZ_LABEL, poliz.size() );
            pl = 0;
        }
    }
    else if (c_type == LEX_BREAK){
        //cout<<"you are in op LEX_BREAK"<<endl;
        if (st_loop.empty()) throw "Using break no in loop";
        get_lex();
        if (c_type != LEX_SEMICOLON) throw curr_lex;
        get_lex();
        pl = poliz.size();
        poliz.push_back ( Lex() );
        poliz.push_back ( Lex(POLIZ_GO) );
    }
    else if (c_type == LEX_GOTO){
        //cout<<"you are in op LEX_GOTO"<<endl;
        get_lex();
        if (c_type != LEX_ID) throw curr_lex;
        label();
        get_lex();
        if (c_type != LEX_SEMICOLON) throw curr_lex;
        get_lex();
    }
    else if (c_type == LEX_READ){
        //cout<<"you are in op LEX_READ"<<endl;
        get_lex();
        if (c_type != LEX_LPAREN) throw curr_lex;
        get_lex();
        if (c_type != LEX_ID) throw curr_lex;
        if (TID[c_val].get_type()!= LEX_STRUCT){
            check_id_in_read ();
            poliz.push_back ( Lex( POLIZ_ADDRESS, c_val) );
            get_lex();
        }
        else {
            c_val_cur = c_val;
            get_lex();
            if (c_type != LEX_POINT) throw curr_lex;
            get_lex();
            if (c_type != LEX_ID) throw curr_lex;
            int k = 0;
            for (k = 0; k < code_struct[TID[c_val_cur].get_value()].field.size(); k++){
                if (TID[c_val].get_name() == code_struct[TID[c_val_cur].get_value()].field[k].get_name()) break;
            }
            if (k == code_struct[TID[c_val_cur].get_value()].field.size()) throw curr_lex;
            if ( !TID[c_val_cur].get_declare() )
                throw "not declared";
           // if (code_struct[TID[c_val_cur].get_value()].field[k].get_type() == LEX_BOOL) throw "wrong types in operation read";
            poliz.push_back ( Lex ( POLIZ_ADDRESS_STRUCT, c_val_cur, k) );
            get_lex();
        }
        if (c_type != LEX_RPAREN) throw curr_lex;
        get_lex();
        if (c_type != LEX_SEMICOLON) throw curr_lex;
        get_lex();
        poliz.push_back ( Lex (LEX_READ) );
    }
    else if (c_type == LEX_WRITE){
        //cout<<"you are in op LEX_WRITE"<<endl;
        get_lex();
        if (c_type != LEX_LPAREN) throw curr_lex;
        get_lex();
        expr();
        while (c_type == LEX_COMMA){
            get_lex();
            expr();
        }
        if (c_type != LEX_RPAREN) throw curr_lex;
        get_lex();
        if (c_type != LEX_SEMICOLON) throw curr_lex;
        get_lex();
        poliz.push_back ( Lex ( LEX_WRITE ) );
    }
    else if (c_type == LEX_FIG_OPEN){ //составной оператор
        //cout<<"you are in op sostavnoy oper"<<endl;
        get_lex();
        Ops();
        if (c_type != LEX_FIG_CL) throw curr_lex;
        get_lex();
    }
    else if (c_type == LEX_ID ){
        //cout<<"you are in op c_type == LEX_ID"<<endl;
        expr();
        if (c_type == LEX_COLON){ //помеченный оператор
            get_lex();
            Op();
        }
        else {
            if (c_type != LEX_SEMICOLON) throw curr_lex;
            get_lex();
        }
    }
    else {
        //cout<<"Error in op"<<endl; 
        throw curr_lex;}
}

void Parser:: func_if(){
   // cout<<"you are in if"<<endl;
    int pl2, pl3;
    if (c_type != LEX_LPAREN) throw curr_lex;
    else{ 
        get_lex();
        expr();
        check_op_1();
        pl2 = poliz.size();
        poliz.push_back ( Lex() );
        poliz.push_back ( Lex(POLIZ_FGO) );
        if (c_type != LEX_RPAREN) throw curr_lex;
        get_lex();
        Op();

        pl3 = poliz.size ();
        poliz.push_back ( Lex () );
 
        poliz.push_back ( Lex ( POLIZ_GO ) );
        poliz[pl2] = Lex ( POLIZ_LABEL, poliz.size() );
        if (c_type != LEX_ELSE) throw curr_lex;
        get_lex();
        Op();
        poliz[pl3] = Lex ( POLIZ_LABEL, poliz.size() );
    }
}
void Parser:: func_for(){
    int pl1,pl2, pl3,pl4;
    //cout<<"you are in for"<<endl;
    if (c_type != LEX_LPAREN) throw curr_lex;
    get_lex();
    if (c_type != LEX_SEMICOLON) {
        expr();
        //check_op_1();
    }
    pl3 = poliz.size ();
    if (c_type != LEX_SEMICOLON) {
        //cout<<"Error 2"<<endl;
        throw curr_lex;
    }
    get_lex();
    if (c_type != LEX_SEMICOLON) {
        expr();
        check_op_1();
    }
    pl1 = poliz.size();
    poliz.push_back ( Lex() );
    poliz.push_back ( Lex(POLIZ_FGO) );
    pl2 = poliz.size();
    poliz.push_back ( Lex() );
    poliz.push_back ( Lex(POLIZ_GO) );
    pl4 = poliz.size();
    if (c_type != LEX_SEMICOLON) {
        //cout<<"Error 1"<<endl;
        throw curr_lex;
    }
    get_lex();
    if (c_type != LEX_RPAREN) {
        //cout<<"Control point 1"<<endl;
        expr();
    }
   // cout<<"Control point 2"<<endl;
    poliz.push_back ( Lex(POLIZ_LABEL, pl3) );
   // cout<<"Control point 3"<<endl;
    poliz.push_back ( Lex(POLIZ_GO));
   // cout<<"Control point 4"<<endl;
    poliz[pl2] = Lex ( POLIZ_LABEL, poliz.size() );
   // cout<<"Control point 5"<<endl;
    if (c_type != LEX_RPAREN) {
       // cout<<"Error 3"<<endl;
        throw curr_lex;
    }
    get_lex();
    Op();
    poliz.push_back ( Lex(POLIZ_LABEL, pl4) );
    poliz.push_back ( Lex(POLIZ_GO) );
    poliz[pl1] = Lex ( POLIZ_LABEL, poliz.size() );

}

void Parser:: func_while(){
    int pl0, pl1;
    pl0 = poliz.size ();
    //cout<<"you are in while"<<endl;
    if (c_type != LEX_LPAREN) throw curr_lex;
    get_lex();
    expr();
    check_op_1();
    pl1 = poliz.size (); 
    poliz.push_back ( Lex () );
    poliz.push_back ( Lex (POLIZ_FGO) );
    if (c_type != LEX_RPAREN) throw curr_lex;
    get_lex();
    Op();
    poliz.push_back ( Lex ( POLIZ_LABEL, pl0 ) );
    poliz.push_back ( Lex ( POLIZ_GO) );
    poliz[pl1] = Lex ( POLIZ_LABEL, poliz.size() );
}

void Parser:: expr(){
    //cout<<"you are in expr"<<endl;
    c_val_cur = c_val;
    E();
    while ((c_type == LEX_EQ) || (c_type == LEX_LSS) || (c_type == LEX_GTR) || (c_type == LEX_LEQ) || (c_type == LEX_NEQ) || (c_type == LEX_GEQ) || (c_type == LEX_EQ_DOUB)){
       // cout<<"you are in expr while"<<endl;
        if (c_type == LEX_EQ){
            c_val = c_val_cur;
            //cout<<c_val<<endl;
            if (TID[c_val].get_type() == LEX_STRUCT){
                if (c_val_f != -1 ) {
                    int i = 0;
                    for (i = 0; i < code_struct[TID[c_val_cur].get_value()].field.size(); i++){
                        if (TID[c_val_f].get_name() == code_struct[TID[c_val_cur].get_value()].field[i].get_name()) break;
                    }
                    if (i == code_struct[TID[c_val_cur].get_value()].field.size()) throw curr_lex;
                    if (TID[c_val].get_declare())
                        st_lex.push ( code_struct[TID[c_val_cur].get_value()].field[i].get_type() );
                    else throw "not declare";
                    poliz.push_back ( Lex ( POLIZ_ADDRESS_STRUCT, c_val, i) );
                    c_val_f = -1;
                }
                else{
                    cout<<"you are here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1"<<endl;
                    check_id_struct();
                    poliz.push_back ( Lex ( POLIZ_ADDRESS_STRUCT, c_val));
                }
            }
            else { 
                check_id();
                poliz.push_back ( Lex ( POLIZ_ADDRESS, c_val ) );
            }
            get_lex();
            expr();   //E()
            eq_type();
            poliz.push_back ( Lex ( LEX_EQ) );
        }
        else{
            st_lex.push ( c_type );
            get_lex();
            E();
            check_op_3();
        }
    }
    //cout<<"you are in expr end"<<endl;cout<<"!";cout<<"!";coucout<<"!";t<<"!";
}

void Parser::E(){
   // cout<<"you are in E()"<<endl;
    T();
    while ((c_type == LEX_PLUS) ||(c_type== LEX_MINUS) || (c_type == LEX_OR) ){
        st_lex.push ( c_type );
        get_lex();
        T();
        check_op_3();
    }
     //cout<<"you are in E() end"<<endl;
} 

void Parser::T(){
    //cout<<"you are in T()"<<endl;
    F();
    while ( (c_type == LEX_TIMES) || (c_type == LEX_SLASH) || (c_type == LEX_AND)){
        st_lex.push ( c_type );
        get_lex();
        F();
        check_op_3 ();
    }
    // cout<<"you are in T() end"<<endl;
} 

void Parser::F(){
   // cout<<"you are in F()"<<endl;
    if (c_type == LEX_ID){
        c_val_cur = c_val;
        if (TID[c_val].get_type() == LEX_STRUCT){
            get_lex();
            if (c_type != LEX_EQ){ 
                if (c_type != LEX_POINT) {//просто структура
                   //  cout<<"****************"<<endl;
                    c_val = c_val_cur;
                  //  cout<<"1"<<"      "<<TID[c_val].get_type()<<endl;
                    check_id_struct();
                    //for (int i = 0; i < code_struct[TID[c_val_cur].get_value()].field.size(); i++){
                         poliz.push_back ( Lex ( LEX_STRUCT, c_val ) );
                     //}
                }
                else{ //поле структуры
                  //  cout<<"@@@@@@@@@@@@@@@"<<endl;
                    get_lex();
                    if (c_type != LEX_ID) throw curr_lex;
                    int c = c_val;
                    c_val_f = c_val;
                    get_lex();
                    if (c_type !=LEX_EQ){
                        c_val = c;
                        int i = 0;
                        for (i = 0; i < code_struct[TID[c_val_cur].get_value()].field.size(); i++){
                            if (TID[c_val].get_name() == code_struct[TID[c_val_cur].get_value()].field[i].get_name()) break;
                        }
                        if (i == code_struct[TID[c_val_cur].get_value()].field.size()) throw curr_lex;
                        if (TID[c_val_cur].get_declare())
                            st_lex.push ( code_struct[TID[c_val_cur].get_value()].field[i].get_type());
                        else throw "not declared";
                        poliz.push_back( Lex (LEX_STRUCT, c_val_cur, i) );
                    }
                }
            }
        }
        else{
            get_lex();
            if (c_type != LEX_COLON) {
                if (c_type != LEX_EQ){
                    c_val = c_val_cur;
                    check_id();
                    poliz.push_back ( Lex ( LEX_ID, c_val ) );
                }
            }
            else{                            //метка
                cout<< "МЕТКА"<<endl;
                st_int.push ( c_val_cur );
                dec(LEX_LAB);
                if ( TID[c_val_cur].get_lab() != -1){
                    poliz[TID[c_val_cur].get_lab() ] = Lex ( POLIZ_LABEL, poliz.size() );
                }
                else{
                    TID[c_val_cur].put_lab(poliz.size()); 
                }
            }
        }
    }
    else if (c_type == LEX_NUM) {
        st_lex.push ( LEX_INT );
        poliz.push_back ( curr_lex );
        get_lex();
    }
    else if ( (c_type == LEX_TRUE) || (c_type == LEX_FALSE) ) {
        st_lex.push ( LEX_BOOL );
        if (c_type == LEX_TRUE) poliz.push_back ( Lex (LEX_TRUE, 1) );
        else poliz.push_back ( Lex (LEX_FALSE, 0) );
        get_lex();
    }
    /*else if (c_type == LEX_NOT){
        get_lex();
        F();

    }*/
    else if (c_type == LEX_LPAREN){
       // cout<<"ok_1"<<endl;
        get_lex();
        expr();
        if (c_type != LEX_RPAREN) {
           // cout<<"Error 1"<<endl;
            throw curr_lex;
        }
        get_lex();
       // cout<<"ok"<<endl;
    }
    else if (c_type == LEX_DOUB_QUOT){ //строка
        get_lex();
        if (c_type != LEX_ID) throw curr_lex;
        poliz.push_back ( Lex(LEX_STR, c_val)  );
        get_lex();
        if (c_type != LEX_DOUB_QUOT) throw curr_lex;
        st_lex.push ( LEX_STR );
        get_lex();
    }
    else if (c_type == LEX_MINUS){
        get_lex();
        F();
        check_not ();
        get_lex();
    }
    else throw curr_lex;
    //cout<<"you are in F() end"<<endl;
} 
////////////////////////////////////////////////////////////////
 
void Parser::dec ( type_of_lex type ) {
    int i;
    while ( !st_int.empty () ) {
        from_st ( st_int, i );
        if ( TID[i].get_declare () ) 
            throw "twice initialization";
        else {
            TID[i].put_declare ();
            TID[i].put_type ( type );
        }
    }
}

void Parser::dec_l(){
    TID[c_val].put_declare ();
}

void Parser::label(){
    if ( !TID[c_val].get_declare() ){
        st_lab.push(c_val);
        TID[c_val].put_lab(poliz.size()); 
        poliz.push_back ( Lex() );
        poliz.push_back ( Lex(POLIZ_GO) );
    }
    else {
        poliz.push_back ( Lex(POLIZ_LABEL, TID[c_val].get_lab()) );
        poliz.push_back ( Lex(POLIZ_GO) );
    }
}

void Parser::check_id () {
    if ( TID[c_val].get_declare() )
        st_lex.push ( TID[c_val].get_type () );
    else 
        throw "not declared";
}
void Parser::check_id_struct () {
    if ( TID[c_val].get_declare() )
        st_lex.push ( static_cast<type_of_lex>(TID[c_val].get_type_of_struct () ));
    else 
        throw "not declared";
}

void Parser::check_op_1 () {
    type_of_lex t;
    from_st ( st_lex, t );
    if (t != LEX_BOOL)  {
        throw "wrong type of operation in if or while of for";
    }
} 
void Parser::check_op_3 () {
    type_of_lex t1, t2, op, t = LEX_INT, r = LEX_BOOL;
 
    from_st ( st_lex, t2 );
    from_st ( st_lex, op );
    from_st ( st_lex, t1 );
    if ( op == LEX_PLUS || op == LEX_MINUS){
        if (t1 == LEX_INT) r = LEX_INT;
        else {
            r = LEX_STR;
            t = LEX_STR;
        }
    }
    if (op == LEX_TIMES || op == LEX_SLASH ){
        r = LEX_INT;
    }
    if ( op == LEX_OR || op == LEX_AND){
        t = LEX_BOOL;
    }
    if (op == LEX_LEQ || op == LEX_GEQ){
        t = LEX_INT;
    }
    if ((op == LEX_LSS) || (op == LEX_GTR) || (op == LEX_EQ_DOUB) || (op == LEX_NEQ)){
        if (t1 == LEX_INT) {}
        else if (t1 == LEX_BOOL) t = LEX_INT;
        else if (t1 == LEX_STR){
            t = LEX_STR;
        }
        r = LEX_BOOL;
    } 
    if ( t1 == t2  &&  t1 == t ) {
        st_lex.push (r);
        //cout<<"r = "<<r<<endl;
    }
    else
     {
            cout<<"t1 = "<<t1<<endl;
            cout<<"t2 = "<<t2<<endl;
            cout<<"op = "<<op<<endl;
            throw "wrong types are in operation";
        }
    poliz.push_back (Lex (op) );
}

void Parser::check_id_in_read () {
    if ( !TID [c_val].get_declare() )
        throw "not declared";
    if (c_type == LEX_BOOL) throw "wrong types in operation read";
}

void Parser::check_not () {
    if (st_lex.top() != LEX_INT)
        throw "wrong type is in not";
    else  {
        poliz.push_back ( Lex (LEX_UN_MINES) );
    }
}

void Parser::eq_type () {
    type_of_lex t;
    from_st ( st_lex, t );
    cout<<t<<" "<<st_lex.top()<<endl;
    if ( t != st_lex.top () )
        throw "wrong types are in =";
    st_lex.pop();
}

////////////////////////////////////////////////////////////////

class Executer {
public:
    void execute ( vector<Lex> & poliz );
};

struct arg {
    int val;
    int field = -1;
    string s;
    bool b; //true - int; false - str
    
    arg(){}
    arg(int v, int f = -1): val(v), field(f), b(true) {}
    arg(string str): s(str), b(false){}
};
 
void Executer::execute ( vector<Lex> & poliz ) {
    Lex pc_el;
    stack < arg > args;
    stack < int > check_args;
    struct arg i,j;
    int index = 0, size = poliz.size(), m,n;
    while ( index < size ) {
        pc_el = poliz [ index ];
        cout <<pc_el<<endl;
        switch ( pc_el.get_type () ) {
            case LEX_TRUE: case LEX_FALSE: case LEX_NUM: case POLIZ_ADDRESS: case POLIZ_LABEL:
                args.push ( arg(pc_el.get_value ()));
                break;

            case POLIZ_ADDRESS_STRUCT:
                args.push ( arg(pc_el.get_value(), pc_el.get_field()));
                break;

            case LEX_STRUCT:
          //  cout<<pc_el.get_field()<< " !!!!!!!!!!!!" <<endl;
                if (pc_el.get_field() != -1) {
                    m = pc_el.get_value ();
                    n = pc_el.get_field();
                    if ( code_struct[TID[m].get_value()].field[n].get_assign () ) {
                        if (!code_struct[TID[m].get_value()].field[n].get_str ().empty()) 
                            args.push ( arg(code_struct[TID[m].get_value()].field[n].get_str ()) );
                        else args.push ( arg(code_struct[TID[m].get_value()].field[n].get_value ()) );
                    }
                    else
                      throw "POLIZ: STRUCT indefinite identifier";
                }
                else{
                    m = pc_el.get_value ();
                    for (int k = 0 ; k < code_struct[TID[m].get_value()].field.size(); k++){
                        if ( !code_struct[TID[m].get_value()].field[k].get_assign () ) throw "POLIZ: STRUCT FIELD indefinite identifier";
                    }
                    args.push ( arg(pc_el.get_value (), -2));
                }
                break;

            case LEX_STR:
                 m = pc_el.get_value ();
                // cout<< TID[m].get_name () <<endl;
                args.push ( arg(TID[m].get_name ()));
                break;
 
            case LEX_ID:
                m = pc_el.get_value ();
                if ( TID[m].get_assign () ) {
                    if (!TID[m].get_str ().empty()) args.push ( arg(TID[m].get_str ()) );
                    else args.push ( arg(TID[m].get_value ()) );
                }
                else
                  throw "POLIZ: indefinite identifier";
              break;
 
            case LEX_UN_MINES:
                from_st ( args, i );
                args.push( -i.val );
                break;
 
            case LEX_OR:
                from_st ( args, i ); 
                from_st ( args, j );
                args.push ( j.val || i.val );
                break;
 
            case LEX_AND:
                from_st ( args, i );
                from_st ( args, j );
                args.push ( j.val && i.val );
                break;
 
            case POLIZ_GO:
                from_st ( args, i );
                index = i.val - 1;
                break;
 
            case POLIZ_FGO:
                from_st ( args, i );
                from_st ( args, j );
                if ( !j.val ) index = i.val - 1;
                break;
 
            case LEX_WRITE:
                from_st ( args, j );
                if (j.b == true)
                    cout << j.val << endl;
                else{
                    //cout<<"write str"<<endl;
                    cout << j.s << endl;
                }
                break;
 
            case LEX_READ:
                int k;
                from_st ( args, i );
                if (i.field == -1){
                    if (i.b == true) {
                        if ( TID[i.val].get_type () == LEX_INT ) {
                            cout << "Input int value for" << TID[i.val].get_name () << endl;
                            cin >> k;
                            TID[i.val].put_value (k);
                             TID[i.val].put_assign ();
                        }
                        else {
                            if (TID[i.val].get_type () == LEX_STR){
                                string s;
                                 cout << "Input string value for " << TID[i.val].get_name() << endl;
                                 cin >> s;
                                // cout << s;
                                 TID[i.val].put_str (s);
                                 TID[i.val].put_assign ();
                            }
                            else{
                                string s;
                                while (1) {
                                    cout << "Input boolean value (true or false) for " << TID[i.val].get_name() << endl;
                                    cin >> s;
                                    if ( s != "true" && s != "false" ) {
                                        cout << "Error in input:true/false" << endl;
                                        continue;
                                    }
                                    k = ( s == "true" ) ? 1 : 0;
                                    break;
                                }
                                TID[i.val].put_value (k);
                                TID[i.val].put_assign ();
                            }
                        }
                    }
                }
                else {//struct
                    if (code_struct[TID[i.val].get_value()].field[i.field].get_type() == LEX_INT){
                        cout << "Input int value for " << TID[i.val].get_name()<<"."<<code_struct[TID[i.val].get_value()].field[i.field].get_name() << endl;
                            cin >> k;
                            code_struct[TID[i.val].get_value()].field[i.field].put_value (k);
                            code_struct[TID[i.val].get_value()].field[i.field].put_assign ();
                    }
                    else if (code_struct[TID[i.val].get_value()].field[i.field].get_type() == LEX_BOOL) {
                        string s;
                        while (1) {
                            cout << "Input boolean value (true or false) for " <<TID[i.val].get_name()<<"."<<code_struct[TID[i.val].get_value()].field[i.field].get_name() << endl;
                            cin >> s;
                            if ( s != "true" && s != "false" ) {
                                cout << "Error in input:true/false" << endl;
                                continue;
                            }
                            k = ( s == "true" ) ? 1 : 0;
                            break;
                        }
                        code_struct[TID[i.val].get_value()].field[i.field].put_value (k);
                        code_struct[TID[i.val].get_value()].field[i.field].put_assign ();
                    }
                    else{
                        string s;
                         cout << "Input string value for " <<TID[i.val].get_name()<<"."<<code_struct[TID[i.val].get_value()].field[i.field].get_name() << endl;
                         cin >> s;
                        // cout << s;
                         code_struct[TID[i.val].get_value()].field[i.field].put_str (s);
                         code_struct[TID[i.val].get_value()].field[i.field].put_assign ();
                    }

                }
                break;
 
            case LEX_PLUS:
                from_st ( args, i );
                from_st ( args, j );
                if (i.b == true)
                    args.push ( arg(i.val + j.val) );
                else args.push(arg(j.s + i.s) );
                break;
 
            case LEX_TIMES:
                from_st ( args, i );
                from_st ( args, j );
                args.push ( arg(i.val * j.val) );
                break;
 
            case LEX_MINUS:
                from_st ( args, i );
                from_st ( args, j );
                args.push ( arg(j.val - i.val) );
                break;
 
            case LEX_SLASH:
                from_st ( args, i );
                from_st ( args, j );
                if (!i.val) {
                    args.push ( arg(j.val / i.val) );
                    break;
                }
                else
                    throw "POLIZ:divide by zero";
 
            case LEX_EQ_DOUB:
                from_st ( args, i );
                from_st ( args, j );
                if (i.b == true)
                    args.push ( arg(i.val == j.val) );
                else {
                    cout<<(i.s == j.s) <<endl;
                    args.push( arg(i.s == j.s));
                }
                break;
 
            case LEX_LSS:
                from_st ( args, i );
                from_st ( args, j );
                if (i.b == true)
                    args.push ( arg(j.val < i.val) );
                else args.push( arg(j.s < i.s));
                break;
 
            case LEX_GTR:
                from_st ( args, i );
                from_st ( args, j );
                if (i.b == true)
                    args.push ( arg(j.val > i.val) );
                else args.push( arg(j.s > i.s));
                break;
 
            case LEX_LEQ:
                from_st ( args, i );
                from_st ( args, j );
                if (i.b == true)
                    args.push ( arg(j.val <= i.val) );
                else args.push( arg(j.s <= i.s));
                break;
 
            case LEX_GEQ:
                from_st ( args, i );
                from_st ( args, j );
               if (i.b == true){
                //cout<< (j.val>=i.val)<<endl;
                    args.push ( arg(j.val >= i.val) );
               }
                else args.push( arg(j.s >=i.s ));
                break;
 
            case LEX_NEQ:
                from_st ( args, i );
                from_st ( args, j );
                if (i.b == true)
                    args.push ( arg(i.val != j.val) );
                else args.push( arg(i.s != j.s));
                break;
 
            case LEX_EQ:
                from_st ( args, i );
                from_st ( args, j );
                if (i.b == true){
                    if ((j.field == -1)&&(i.field!= -2)){
                        //cout<<"1"<<endl;
                        TID[j.val].put_value (i.val);
                        TID[j.val].put_assign ();
                        args.push(arg(i.val)) ;
                    }
                    else{
                        if (i.field == -2){
                            //cout<<"2"<<endl;
                            for (int k = 0; k < code_struct[i.val].field.size(); k++){
                                code_struct[j.val].field[k].put_value(code_struct[i.val].field[k].get_value());
                                //cout<<"field = "<<code_struct[i.val].field[k].get_value()<<endl;
                                code_struct[j.val].field[k].put_assign();
                            }
                        }
                        else{
                            //cout<<"3"<<endl;
                           // cout<<"j.field = "<<j.field<<endl;
                            code_struct[j.val].field[j.field].put_value (i.val);
                            code_struct[j.val].field[j.field].put_assign();
                            args.push(arg(i.val)) ;
                        }
                    }
                }
                else {
                    //cout<<"str"<<i.s<<endl;
                    if (j.field == -1){
                        TID[j.val].put_str (i.s);
                        TID[j.val].put_assign ();
                         args.push(arg(i.s)) ;
                     }
                     else{
                        code_struct[j.val].field[j.field].put_str (i.s);
                        code_struct[j.val].field[j.field].put_assign();
                        args.push(arg(i.s)) ;
                     }
                } 
                break;
 
            default:
                throw "POLIZ: unexpected elem";
        }//end of switch
        ++index;
    };//end of while
    cout << "Finish of executing!!!" << endl;
}
 
class Interpretator {
    Parser   pars;
    Executer E;
public:
    Interpretator ( const char* program ): pars (program) {}
    void     interpretation ();
};
 
void Interpretator::interpretation () {
    pars.analyze ();
    E.execute ( pars.poliz );
}
 
int main (int argc, char* argv[]) {
    try {
        Interpretator I ( argv[1] );
        I.interpretation ();
        return 0;
    }
    catch ( char c ) {
        cout << "unexpected symbol " << c << endl;
        return 1;
    }
    catch ( Lex l ) {
        cout << "unexpected lexeme" << l << endl;
        return 1;
    }
    catch ( const char *source ) {
        cout << source << endl;
        return 1;
    }
}

