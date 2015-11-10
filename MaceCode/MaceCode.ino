const String Mace_A = "=";
const String Mace_B = "^^=";
const String Mace_C = "^=$";
const String Mace_D = "=^";
const String Mace_E = "^";
const String Mace_F = "^=^";
const String Mace_G = "==";
const String Mace_H = "^==";
const String Mace_I = "^^";
const String Mace_J = "=^^";
const String Mace_K = "^$";
const String Mace_L = "^^$";
const String Mace_M = "^=";
const String Mace_N = "=$";;
const String Mace_O = "$=";
const String Mace_P = "$^";
const String Mace_Q = "=^$";
const String Mace_R = "^$=";
const String Mace_S = "^$^";
const String Mace_T = "$";
const String Mace_U = "$$";
const String Mace_V = "==^";
const String Mace_W = "==$";
const String Mace_X = "=$^";
const String Mace_Y = "^$$";
const String Mace_Z = "===";
const String Mace_1 = "$^^=";
const String Mace_2 = "$^^$";
const String Mace_3 = "$^=^";
const String Mace_4 = "$^==";
const String Mace_5 = "$^=$";
const String Mace_6 = "$^$^";
const String Mace_7 = "$^$=";
const String Mace_8 = "$^$$";
const String Mace_9 = "$=^^";
const String Mace_0 = "$^^^";
const String Mace_Period = "$^^";
const String Mace_Comma = "$^=";
const String Mace_Apostrophe = "$=^";
const String Mace_Question = "$==";
const String Mace_Exclamation = "$=$";
const String Mace_Plus = "$$^";
const String Mace_Minus = "$$=";
const String Mace_Asterix = "$$$";
const String Mace_Divide = "$^$";
const String Mace_Equals = "$===$";

//#define DEBUG
//////////////// defining pins
#define RED_LED 5
#define AMBER_LED 6
#define YELLOW_LED 9
#define GREEN_LED 10
#define BLUE_LED 11
#define IR_LED A1
#define IR_REC 2
#define POT 0

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //Initialise the pins
  pinMode(RED_LED, OUTPUT);
  pinMode(AMBER_LED, OUTPUT);
  pinMode(IR_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(IR_REC, INPUT);
  pinMode(POT, INPUT);
  digitalWrite(A1, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  String input = getASCIIInput();
#ifdef DEBUG
  Serial.println("Got Input");
#endif
  if (input.charAt(0) == '$' || input.charAt(0) == '^' || input.charAt(0) == '=')
  {
    // If Recieved MACE
    respondToMace(input);
  }
  else
  {
    // If not recieved MACE
    String mace = stringToMace(input);
#ifdef DEBUG
    Serial.println("Converted to MACE");
#endif
    Serial.println(mace);
    sendDigital(11, mace);
  }
}
void respondToMace(String mace)
{
  String convertedMace = maceToString(mace);
#ifdef DEBUG
  Serial.println("Responding to MACE");
#endif

  if (mace.startsWith("LP"))
  {
#ifdef DEBUG
    Serial.println("LP");
#endif
    for (int i = 2; i < convertedMace.length(); i += 3)
    {
      // get the block of data
      String block = convertedMace.substring(i, i + 2);
      int pwmValue = block.toInt();
      switch (i)
      {
        case 2:
          analogWrite(RED_LED, pwmValue);
          break;
        case 5:
          analogWrite(AMBER_LED, pwmValue);
          break;
        case 8:
          analogWrite(YELLOW_LED, pwmValue);
          break;
        case 11:
          analogWrite(GREEN_LED, pwmValue);
          break;
        default:
          ;
          break;
      }
    }
  }
  else if (convertedMace.startsWith("+-IR"))
  {
#ifdef DEBUG
    Serial.println("+-IR");
#endif
    digitalWrite(IR_LED, HIGH);
    delay(1000);
    digitalWrite(IR_LED, LOW);
  }
  else if (convertedMace.startsWith("RXIR"))
  {
#ifdef DEBUG
    Serial.println("RXIR");
#endif
    int inVal = digitalRead(IR_REC);
    if (inVal)
    {
      Serial.println("LOW");
    }
    else
    {
      Serial.println("HIGH");
    }
  }
  else if (convertedMace.startsWith("VRV"))
  {
#ifdef DEBUG
    Serial.println("VRV");
#endif
    String inVal = String(analogRead(POT), DEC);

    // While length < 4 add a 0 to the start of the string
    while (inVal.length() < 4)
    {
      String zero = "0";
      inVal = "0" + inVal;
    }
    Serial.println(stringToMace(inVal));

  }
}
String getASCIIInput()
{
  String retVal = "";
  char inputChar;
  while (inputChar != '\n')
  {
    while (!Serial.available())
    {
      ;
    }
    inputChar = Serial.read();
    if (inputChar != '\n') {
      retVal = retVal + inputChar;
    }
  }
  return retVal;
}

String stringToMace(String s)
{
  String retVal = "";
  for (int i = 0; i < s.length(); i++)
  {
    retVal += charToMace(s.charAt(i));
    retVal += '/';
  }
  return retVal;
}

String maceToString(String s)
{
  int startIndex = 0;
  String retVal = "";
  int endIndex = 0;
  // Loop until no more slashes are found, indexOf() returns -1 when no more of that character remain.
  while (endIndex != -1)
  {
    endIndex = s.indexOf('/', startIndex + 1);

    //Serial.print("BreakPoint: "); Serial.println(endIndex);
    // Get the substring between the two slashes
    String sstring = s.substring(startIndex, endIndex);
    // Remove remaining slashes
    sstring.replace("/", "");
    char maceChar = maceToChar(sstring);
    retVal = retVal + maceChar;
    startIndex = endIndex;
  }
  return retVal;
}
void sendDigital(int pin, String message)
{
  int timeUnit;
  // timeUnit has a base value of 20 and has the potentiometer value modified to make up the remaining 480 to a maximum of 500.
  timeUnit = 20 + (analogRead(POT) * (float(480) / float(1023)));

  for (int i = 0; i < message.length(); i++)
  {
    if (message.charAt(i) == '^')
    {
      digitalWrite(pin, HIGH);
      delay(timeUnit);
      digitalWrite(pin, LOW);
    }
    else if (message.charAt(i) == '=')
    {
      digitalWrite(pin, HIGH);
      delay(2 * timeUnit);
      digitalWrite(pin, LOW);
    }
    else if (message.charAt(i) == '$')
    {
      // 4 + 1 = 5
      delay(4 * timeUnit);
    }
    else if (message.charAt(i) == '/')
    {
      // 2 + 1 = 3
      delay(2 * timeUnit);
    }
    else if (message.charAt(i) == ' ')
    {
      // 4 + 1  = 5
      delay(4 * timeUnit);
    }
    // This delay is the default delay between signals and a additional delay to the previous delays between characters
    // and words (/ and space)
    delay(timeUnit);
  }
}

//////////////////////////////////////// LONG CONVERSIONS ///////////////////////////////////////////////////////////////////////////////


String charToMace(char c)
{
  switch (tolower(c))
  {
    case 'a':
      return Mace_A;
      break;
    case 'b':
      return Mace_B;
      break;
    case 'c':
      return Mace_C;
      break;
    case 'd':
      return Mace_D;
      break;
    case 'e':
      return Mace_E;
      break;
    case 'f':
      return Mace_F;
      break;
    case 'g':
      return Mace_G;
      break;
    case 'h':
      return Mace_H;
      break;
    case 'i':
      return Mace_I;
      break;
    case 'j':
      return Mace_J;
      break;
    case 'k':
      return Mace_K;
      break;
    case 'l':
      return Mace_L;
      break;
    case 'm':
      return Mace_M;
      break;
    case 'n':
      return Mace_N;
      break;
    case 'o':
      return Mace_O;
      break;
    case 'p':
      return Mace_P;
      break;
    case 'q':
      return Mace_Q;
      break;
    case 'r':
      return Mace_R;
      break;
    case 's':
      return Mace_S;
      break;
    case 't':
      return Mace_T;
      break;
    case 'u':
      return Mace_U;
      break;
    case 'v':
      return Mace_V;
      break;
    case 'w':
      return Mace_W;
      break;
    case 'x':
      return Mace_X;
      break;
    case 'y':
      return Mace_Y;
      break;
    case 'z':
      return Mace_Z;
      break;
    case '1':
      return Mace_1;
      break;
    case '2':
      return Mace_2;
      break;
    case '3':
      return Mace_3;
      break;
    case '4':
      return Mace_4;
      break;
    case '5':
      return Mace_5;
      break;
    case '6':
      return Mace_6;
      break;
    case '7':
      return Mace_7;
      break;
    case '8':
      return Mace_8;
      break;
    case '9':
      return Mace_9;
      break;
    case '0':
      return Mace_0;
      break;
    case '.':
      return Mace_Period;
      break;
    case ',':
      return Mace_Comma;
      break;
    case '\'':
      return Mace_Apostrophe;
      break;
    case '?':
      return Mace_Question;
      break;
    case '!':
      return Mace_Exclamation;
      break;
    case '+':
      return Mace_Plus;
      break;
    case '-':
      return Mace_Minus;
      break;
    case '*':
      return Mace_Asterix;
      break;
    case '/':
      return Mace_Divide;
      break;
    case '=':
      return Mace_Equals;
      break;
    case ' ':
      return " ";
      break;
  }
}

char maceToChar(String mace)
{
  if (mace == Mace_A)
  {
    return 'A';
  }
  else if (mace == Mace_B)
  {
    return 'B';
  }
  else if (mace == Mace_C)
  {
    return 'C';
  }
  else if (mace == Mace_D)
  {
    return 'D';
  }
  else if (mace == Mace_E)
  {
    return 'E';
  }
  else if (mace == Mace_F)
  {
    return 'F';
  }
  else if (mace == Mace_G)
  {
    return 'G';
  }
  else if (mace == Mace_H)
  {
    return 'H';
  }
  else if (mace == Mace_I)
  {
    return 'I';
  }
  else if (mace == Mace_J)
  {
    return 'J';
  }
  else if (mace == Mace_K)
  {
    return 'K';
  }
  else if (mace == Mace_L)
  {
    return 'L';
  }
  else if (mace == Mace_M)
  {
    return 'M';
  }
  else if (mace == Mace_N)
  {
    return 'N';
  }
  else if (mace == Mace_O)
  {
    return 'O';
  }
  else if (mace == Mace_P)
  {
    return 'P';
  }
  else if (mace == Mace_Q)
  {
    return 'Q';
  }
  else if (mace == Mace_R)
  {
    return 'R';
  }
  else if (mace == Mace_S)
  {
    return 'S';
  }
  else if (mace == Mace_T)
  {
    return 'T';
  }
  else if (mace == Mace_U)
  {
    return 'U';
  }
  else if (mace == Mace_V)
  {
    return 'V';
  }
  else if (mace == Mace_W)
  {
    return 'W';
  }
  else if (mace == Mace_X)
  {
    return 'X';
  }
  else if (mace == Mace_Y)
  {
    return 'Y';
  }
  else if (mace == Mace_Z)
  {
    return 'Z';
  }
  else if (mace == Mace_1)
  {
    return '1';
  }
  else if (mace == Mace_2)
  {
    return '2';
  }
  else if (mace == Mace_3)
  {
    return '3';
  }
  else if (mace == Mace_4)
  {
    return '4';
  }
  else if (mace == Mace_5)
  {
    return '5';
  }
  else if (mace == Mace_6)
  {
    return '6';
  }
  else if (mace == Mace_7)
  {
    return '7';
  }
  else if (mace == Mace_8)
  {
    return '8';
  }
  else if (mace == Mace_9)
  {
    return '9';
  }
  else if (mace == Mace_0)
  {
    return '0';
  }
  else if (mace == Mace_Period)
  {
    return '.';
  }
  else if (mace == Mace_Comma)
  {
    return ',';
  }
  else if (mace == Mace_Apostrophe)
  {
    return '\'';
  }
  else if (mace == Mace_Question)
  {
    return '?';
  }
  else if (mace == Mace_Exclamation)
  {
    return '!';
  }
  else if (mace == Mace_Plus)
  {
    return '+';
  }
  else if (mace == Mace_Minus)
  {
    return '-';
  }
  else if (mace == Mace_Asterix)
  {
    return '*';
  }
  else if (mace == Mace_Divide)
  {
    return '/';
  }
  else if (mace == Mace_Equals)
  {
    return '=';
  }
  else if (mace == " ")
  {
    return ' ';
  }
}

