import java.math.BigDecimal;
import java.math.BigInteger;
import java.util.Scanner;
import java.util.function.ToIntFunction;

public class InflixToPostfix {
    String validOperators = "+-*/";
    String validNumbers = "0123456789";
    String validParentheses = "()";
    Queue equation = new Queue();
    int currentPlacement;
    int numberStart = -1;
    int numberEnd = -1;
    //TotalNumber is optional but might be used for debugging.
    int totalNumberOfNumbers = 0;
    int totalNumberOfOperators = 0;
    int totalNumberOfLeftParentheses = 0;
    int totalNumberOfRightParentheses = 0;

    /**
     * Kyle Hinkle
     * Main Method
     * @param args
     */
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Enter infix expression: ");
        String userInput = scanner.nextLine();
        InflixToPostfix main = new InflixToPostfix();
        main.stringToQueue(userInput);
    }

    /**
     * Kyle Hinkle
     * Converts the userInputted string and converts it to a Queue list.
     * @param userInput
     */
    public void stringToQueue(String userInput){
        Boolean realEquation = true;
        for(currentPlacement = 0; currentPlacement < userInput.length() &  realEquation; currentPlacement++) {

            //Checks to see if its a number
            if (isNumber(currentPlacement, userInput)) {
                if (numberStart < 0) {
                    numberStart = currentPlacement;
                    numberEnd = currentPlacement;
                } else {
                    numberEnd = currentPlacement;
                }
            } else if (isOperator(currentPlacement, userInput)) { //Check for valid Operator
                endOfNumber(userInput);
                totalNumberOfOperators++;
                equation.enqueue(userInput.substring(currentPlacement, currentPlacement + 1));
            } else if (isParenthesis(currentPlacement, userInput)) { // Check for a Parenthesis
                endOfNumber(userInput);
                equation.enqueue(userInput.substring(currentPlacement, currentPlacement + 1));
            } else if (userInput.substring(currentPlacement, currentPlacement + 1).equals("")) { //Check if Empty
                endOfNumber(userInput);
                totalNumberOfNumbers++;
            } else if(currentPlacement%2 == 0){ //Throws out error if received an invalid operand when it was expecting one.
                System.out.println("Invalid Operand");
                realEquation = false;
            }else if(currentPlacement%2 == 1){ //Throws out error if received an invalid operator when it was expecting one.
                System.out.println("Invalid Operator: " + userInput.substring(currentPlacement, currentPlacement + 1));
                realEquation = false;
            }
        }
        if(totalNumberOfRightParentheses > totalNumberOfLeftParentheses){ //Throws out an error is there is an extra right parentheses
            System.out.println("Unbalanced right parenthesis ')'");
            realEquation=false;
        }else if(totalNumberOfLeftParentheses != totalNumberOfRightParentheses){ //Throws out an error if there is an extra left parenthesis with no connecting right parenthesis.
            System.out.println("Unbalanced left parenthesis '('");
            realEquation = false;
        }else if(currentPlacement%2 == 0){ //Throws out error if missing an ending Operand
            System.out.println("Missing Operand: ");
            realEquation = false;
        }
        if(realEquation) {
            endOfNumber(userInput);
            System.out.println(standardizedInfix());
            System.out.println(postfixExpression());
            System.out.println(answer(equation));
        }
    }

    /**
     * Kyle Hinkle
     * Prints out the Queue list based off of the user Input.
     */
    protected String standardizedInfix() {
        String standardized = "Standardized infix: ";
        standardized += equation.toString();
        return standardized;
    }

    /**
     * Kyle Hinkle
     * Does Something
     * @return
     */
    protected String postfixExpression() {
        String postfixExpression = "Postfix expression: ";

        return postfixExpression;
    }

    /**
     * Kyle Hinkle
     * Displays the answer of the equation
     * This answer only works if everything is left to right and there are no parenthesis
     *
     * Unfortunately this code is flawed and doesn't do what was requested.
     * As such it will be deleted by the time we submit.
     * Until then, it still does the basics of what we want.
     */
    public String answer(Queue equation) {
        String answer = "Answer: ";
        int size = equation.size();
        Queue temp = equation;
        double firstNumber = 0;
        double secondNumber = 0;
        String currentOperator = "";

        for(int i = 0; i < size; i++) {
            String currentQueue = (String) temp.dequeue();
            if (i == 0){
                while(isParenthesis(0,currentQueue)){ //Skip till you get to a real number
                    currentQueue = (String) temp.dequeue();
                    i++;
                }
                firstNumber = Double.parseDouble(currentQueue);
            }else if(currentQueue.equals("+")){
                currentOperator = "+";
            } else if(currentQueue.equals("-")){
                currentOperator = "-";
            } else if(currentQueue.equals("*")){
                currentOperator = "*";
            } else if(currentQueue.equals("/")){
                currentOperator = "/";
            }else if(isParenthesis(0,currentQueue)){
                //Do Nothing
            } else {
                secondNumber = Integer.parseInt(currentQueue);
                if(currentOperator.equals("+")){
                    firstNumber = firstNumber + secondNumber;
                } else if(currentOperator.equals("-")){
                        firstNumber = firstNumber - secondNumber;
                } else if(currentOperator.equals("*")){
                    firstNumber = firstNumber * secondNumber;
                } else if(currentOperator.equals("/")) {
                    firstNumber = firstNumber / secondNumber;
                }
            }
        }
        answer += firstNumber;
        return answer;
    }

    /**
     * Kyle Hinkle
     * Checks for a valid number and if there is one it saves it to the list.
     * @param userInput
     */
    private void endOfNumber(String userInput) {
        if(numberStart >=0){
            equation.enqueue(userInput.substring(numberStart, numberEnd+1));
            numberStart = -1;
            numberEnd = -1;
        }
    }

    /**
     * Kyle Hinkle
     * Checks to see if the character is a number.
     * @param currentPlacement
     * @param userInput
     * @return
     */
    public boolean isNumber(int currentPlacement, String userInput){
        for(int i = 0; validNumbers.length()>i; i++){
            if(userInput.substring(currentPlacement, currentPlacement+1).equals(validNumbers.substring(i, i+1)) || userInput.substring(currentPlacement, currentPlacement+1).equals(".")){
                return true;
            }
        }
        return false;
    }

    /**
     * Kyle Hinkle
     * Checks to see if the character is a Operator.
     * @param currentPlacement
     * @param userInput
     * @return
     */
    public boolean isOperator(int currentPlacement, String userInput){
        for(int i = 0; validOperators.length()>i; i++){
            if(userInput.substring(currentPlacement, currentPlacement+1).equals(validOperators.substring(i, i+1))){
                return true;
            }
        }
        return false;
    }

    /**
     * Kyle Hinkle
     * Takes in a string value and converts it into scientific Notation.
     * @param value
     * @return
     */
    /*
    Turns out we don't need this.
    public String scientificNotation(String value){
        String stringValue = value;
        int zeroAmount;
        for(zeroAmount =0; zeroAmount<stringValue.length() && stringValue.charAt(stringValue.length()-(zeroAmount+1)) == '0'; zeroAmount++){

        }
        String afterDecimal = stringValue.substring(1, stringValue.length()-zeroAmount);
        String notated;
        if(afterDecimal == ""){
            notated = stringValue.charAt(0) + "." + "0" +"E" + (stringValue.length() - 1);
        }else {
            notated = stringValue.charAt(0) + "." + afterDecimal +"E" + (stringValue.length() - 1);
        }
        return notated;
    }

     */

    /**
     * Kyle Hinkle
     * Checks to see if the character is a parenthesis.
     * @param currentPlacement
     * @param userInput
     * @return
     */
    public boolean isParenthesis(int currentPlacement, String userInput){
        for(int i = 0; validParentheses.length()>i; i++){
            String value = userInput.substring(currentPlacement, currentPlacement+1);
            if(value.equals(validParentheses.substring(i, i+1))){
                if(value.equals("(")){
                    totalNumberOfLeftParentheses++;
                }else{
                    totalNumberOfRightParentheses++;
                }
                return true;
            }
        }
        return false;
    }
}