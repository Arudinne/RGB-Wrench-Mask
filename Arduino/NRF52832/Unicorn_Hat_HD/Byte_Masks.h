// Byte masks for facial expressions

#define ROW_SIZE 16

//  X_X - Face001

byte face001_Left [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0},
  {0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0},
  {0,0,0,0,0,1,1,0,0,1,1,1,0,0,0,0},
  {0,0,0,0,0,1,1,1,0,1,1,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0},
  {0,0,0,0,0,1,1,1,0,1,1,1,0,0,0,0},
  {0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0},
  {0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0},
  {0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

byte face001_Right [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0},
  {0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0},
  {0,0,0,0,1,1,1,0,0,1,1,0,0,0,0,0},
  {0,0,0,0,0,1,1,0,1,1,1,0,0,0,0,0},
  {0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,1,1,1,0,1,1,1,0,0,0,0,0},
  {0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0},
  {0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0},
  {0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};
 
// >_< - Face002

byte face002_Left [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0},
  {0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0},
  {0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

byte face002_Right [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0},
  {0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0},
  {0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
  {0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

// ^_^ - Face003

byte face003_Left [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0},
  {0,0,0,0,1,1,1,0,1,1,1,0,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0},
  {0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0},
  {0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

byte face003_Right [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0},
  {0,0,0,0,0,1,1,1,0,1,1,1,0,0,0,0},
  {0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0},
  {0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0},
  {0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

// @_@ - Face004

byte face004_Left [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0},
  {0,0,0,1,1,0,0,0,1,0,0,0,0,1,0,0},
  {0,0,1,1,0,0,1,1,1,1,0,1,0,1,0,0},
  {0,0,1,1,0,1,1,0,0,0,1,1,0,1,0,0},
  {0,0,1,1,0,1,1,0,0,0,1,0,0,1,0,0},
  {0,0,1,1,0,1,0,0,0,0,1,0,0,1,0,0},
  {0,0,1,1,0,1,0,0,0,1,1,0,1,1,0,0},
  {0,0,1,1,0,1,1,1,1,1,1,1,1,0,0,0},
  {0,0,0,1,1,0,1,1,0,0,1,0,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0},
  {0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

byte face004_Right [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0},
  {0,0,0,1,1,0,0,0,1,0,0,0,0,1,0,0},
  {0,0,1,1,0,0,1,1,1,1,0,1,0,1,0,0},
  {0,0,1,1,0,1,1,0,0,0,1,1,0,1,0,0},
  {0,0,1,1,0,1,1,0,0,0,1,0,0,1,0,0},
  {0,0,1,1,0,1,0,0,0,0,1,0,0,1,0,0},
  {0,0,1,1,0,1,0,0,0,1,1,0,1,1,0,0},
  {0,0,1,1,0,1,1,1,1,1,1,1,1,0,0,0},
  {0,0,0,1,1,0,1,1,0,0,1,0,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0},
  {0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

// /_\ - Face005

byte face005_Left [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0},
  {0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0},
  {0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

byte face005_Right [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

// ?_? - Face006

byte face006_Left [][ROW_SIZE] = {
  {0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0},
  {0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0},
  {0,0,0,1,1,0,0,0,0,1,1,1,0,0,0,0},
  {0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
};

byte face006_Right [][ROW_SIZE] = {
  {0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0},
  {0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0},
  {0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0},
  {0,0,0,0,1,1,0,0,0,0,1,1,1,0,0,0},
  {0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
};
 
// O_o - Face007

byte face007_Left [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0},
  {0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0},
  {0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0},
  {0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0},
  {0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0},
  {0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0},
  {0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0},
  {0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0},
  {0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0},
  {0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0},
  {0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0},
  {0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

byte face007_Right [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0},
  {0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0},
  {0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0},
  {0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};
 
// \_/ - Face008

byte face008_Left [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

byte face008_Right [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0},
  {0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
  {0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};
 
// Z_Z - Face009

byte face009_Left [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0},
  {0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0},
  {0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

byte face009_Right [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0},
  {0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0},
  {0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

// #_# - Face010

byte face010_Left [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0},
  {0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0},
  {0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0},
  {0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0},
  {0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0},
  {0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0},
  {0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},
  {0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},
  {0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},
  {0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0},
  {0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},
  {0,0,0,0,0,1,0,0,0,1,1,0,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0},
  {0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0},
  {0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0},
};

byte face010_Right [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0},
  {0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0},
  {0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0},
  {0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0},
  {0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0},
  {0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0},
  {0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},
  {0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},
  {0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},
  {0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0},
  {0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},
  {0,0,0,0,0,1,0,0,0,1,1,0,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0},
  {0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0},
  {0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0},
};
 
// =_= - Face011

byte face011_Left [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0},
  {0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0},
  {0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

byte face011_Right [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0},
  {0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0},
  {0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};
 
// ~_^ - Face012

byte face012_Left [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,1,1,1,0,0,0,0,0,1,0,0},
  {0,0,0,0,1,1,1,1,1,1,1,0,1,1,0,0},
  {0,0,0,0,1,0,0,0,1,1,1,1,1,1,0,0},
  {0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

byte face012_Right [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0},
  {0,0,0,0,0,1,1,1,0,1,1,1,0,0,0,0},
  {0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0},
  {0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0},
  {0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

// 9_9 - Face013

byte face013_Left [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0},
  {0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0},
  {0,0,0,0,1,1,1,0,0,0,1,1,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0},
  {0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0},
  {0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0},
  {0,0,0,0,0,1,1,1,0,1,1,1,1,0,0,0},
  {0,0,0,0,0,0,1,1,1,1,0,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0},
  {0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0},
  {0,0,0,0,1,1,0,0,1,1,1,1,0,0,0,0},
  {0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

byte face013_Right [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0},
  {0,0,0,1,1,1,0,0,0,1,1,0,0,0,0,0},
  {0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0},
  {0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0},
  {0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0},
  {0,0,0,0,1,1,1,0,1,1,1,1,0,0,0,0},
  {0,0,0,0,0,1,1,1,1,0,1,1,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0},
  {0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0},
  {0,0,0,1,1,0,0,1,1,1,1,0,0,0,0,0},
  {0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};
 
// !_! - Face014

byte face014_Left [][ROW_SIZE] = {
  {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

byte face014_Right [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

// *_* - Face015

byte face015_Left [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,1,1,0,1,0,1,1,0,0,0,0,0},
  {0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

byte face015_Right [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,1,1,0,1,0,1,1,0,0,0,0},
  {0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

// `_` - Face016

byte face016_Left [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

byte face016_Right [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};
 
// ;_; - Face017

byte face017_Left [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

byte face017_Right [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

// .)_.) - Face018

byte face018_Left [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0},
  {0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0},
  {0,0,0,0,0,1,1,0,0,0,1,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0},
};

byte face018_Right [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0},
  {0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0},
  {0,0,0,0,0,1,1,0,0,0,1,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0},
};

// (._(. - Face019

byte face019_Left [][ROW_SIZE] = {
  {0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0},
  {0,0,0,1,1,1,0,0,0,1,1,0,0,0,0,0},
  {0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0},
};

byte face019_Right [][ROW_SIZE] = {
  {0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0},
  {0,0,0,1,1,1,0,0,0,1,1,0,0,0,0,0},
  {0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0},
};

// o_O - Face020

byte face020_Left [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0},
  {0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0},
  {0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0},
  {0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

byte face020_Right [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0},
  {0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0},
  {0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0},
  {0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0},
  {0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0},
  {0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0},
  {0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0},
  {0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0},
  {0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0},
  {0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0},
  {0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0},
  {0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

// ._. - Face021

byte face021_Left [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

byte face021_Right [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

// ⌐_⌐ - Face022

byte face022_Left [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0},
  {0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

byte face022_Right [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0},
  {0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

// <3_<3 - Face023

byte face023_Red [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0},
  {0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
  {0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0},
  {0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

byte face023_White [][ROW_SIZE] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

// Umbrella Logo

byte face024_Red [][ROW_SIZE] = {
  {0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0},
  {0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
  {1,1,0,0,0,0,0,1,1,0,0,0,0,0,1,1},
  {0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0},
  {0,1,1,1,1,1,0,0,0,0,1,1,1,1,1,0},
  {0,1,1,1,1,1,0,0,0,0,1,1,1,1,1,0},
  {0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0},
  {1,1,0,0,0,0,0,1,1,0,0,0,0,0,1,1},
  {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0},
  {0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0},
  {0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0},
};

byte face024_White [][ROW_SIZE] = {
  {0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0},
  {0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0},
  {0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0},
  {1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1},
  {0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0},
  {0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0},
  {0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0},
  {1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1},
  {0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0},
  {0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0},
  {0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0},
};