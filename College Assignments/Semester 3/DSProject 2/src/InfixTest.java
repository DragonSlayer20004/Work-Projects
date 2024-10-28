import org.junit.jupiter.api.*;

public class InfixTest {

    @BeforeAll
    public static void setQueue(){

    }
    @Test
    public void standardizedInfixTestOne(){
        InflixToPostfix testing = new InflixToPostfix();
        testing.equation.enqueue("20");
        testing.equation.enqueue("/");
        testing.equation.enqueue("10"); //2
        testing.equation.enqueue("+");
        testing.equation.enqueue("3"); //5

        Assertions.assertEquals("Standardized infix: 20 / 10 + 3 ", testing.standardizedInfix());
    }
    @Test
    public void standardizedInfixTestTwo(){
        InflixToPostfix testing = new InflixToPostfix();
        testing.equation.enqueue("(");
        testing.equation.enqueue("20");
        testing.equation.enqueue("/");
        testing.equation.enqueue("(");
        testing.equation.enqueue("10"); //2
        testing.equation.enqueue(")");
        testing.equation.enqueue("+");
        testing.equation.enqueue("3"); //5
        testing.equation.enqueue(")");

        Assertions.assertEquals("Standardized infix: ( 20 / ( 10 ) + 3 ) ", testing.standardizedInfix());
    }
    @Test
    public void standardizedInfixTestThree(){
        InflixToPostfix testing = new InflixToPostfix();
        testing.equation.enqueue("10");
        testing.equation.enqueue("/");
        testing.equation.enqueue("10"); //2
        testing.equation.enqueue("+");
        testing.equation.enqueue("10"); //5

        Assertions.assertEquals("Standardized infix: 10 / 10 + 10 ", testing.standardizedInfix());
    }

    @Test
    public void stringToQueueTestOne(){
        InflixToPostfix testing = new InflixToPostfix();
        testing.equation.enqueue("20");
        testing.equation.enqueue("/");
        testing.equation.enqueue("10"); //2
        testing.equation.enqueue("+");
        testing.equation.enqueue("3"); //5.0

        Assertions.assertEquals("Answer: 5.0", testing.answer(testing.equation));
    }
    @Test
    public void stringToQueueTestTwo(){
        InflixToPostfix testing = new InflixToPostfix();
        testing.equation.enqueue("100");
        testing.equation.enqueue("/");
        testing.equation.enqueue("10"); //10
        testing.equation.enqueue("+");
        testing.equation.enqueue("10"); //20

        Assertions.assertEquals("Answer: 20.0", testing.answer(testing.equation));
    }
    @Test
    public void stringToQueueTestThree(){
        InflixToPostfix testing = new InflixToPostfix();
        testing.equation.enqueue("3");
        testing.equation.enqueue("*");
        testing.equation.enqueue("3"); //9
        testing.equation.enqueue("+");
        testing.equation.enqueue("3"); //12

        Assertions.assertEquals("Answer: 12.0", testing.answer(testing.equation));
    }
    @Test
    public void stringToQueueTestFour(){
        InflixToPostfix testing = new InflixToPostfix();
        testing.equation.enqueue("1");
        testing.equation.enqueue("/");
        testing.equation.enqueue("1"); //1
        testing.equation.enqueue("/");
        testing.equation.enqueue("1"); //1

        Assertions.assertEquals("Answer: 1.0", testing.answer(testing.equation));
    }
    @Test
    public void stringToQueueTestFive(){
        InflixToPostfix testing = new InflixToPostfix();
        testing.equation.enqueue("1000000000000000");
        testing.equation.enqueue("*");
        testing.equation.enqueue("2"); //2000000000000000
        testing.equation.enqueue("*");
        testing.equation.enqueue("3"); //6000000000000000

        Assertions.assertEquals("Answer: 6.0E15", testing.answer(testing.equation));
    }
    @Test
    public void isNumberTest(){
        InflixToPostfix testing = new InflixToPostfix();
        String input = "10 + 10";

        Assertions.assertEquals(true, testing.isNumber(0, input));
        Assertions.assertEquals(true, testing.isNumber(1, input));
        Assertions.assertEquals(true, testing.isNumber(5, input));
        Assertions.assertEquals(true, testing.isNumber(6, input));
    }
    @Test
    public void isParenthesisTest(){
        InflixToPostfix testing = new InflixToPostfix();
        String input = "-(10 + 10)";

        Assertions.assertEquals(true, testing.isParenthesis(1, input));
        Assertions.assertEquals(true, testing.isParenthesis(9, input));
    }
    @Test
    public void isOperatorTest(){
        InflixToPostfix testing = new InflixToPostfix();
        String input = "-(10 + 10)";

        Assertions.assertEquals(true, testing.isOperator(0, input));
        Assertions.assertEquals(true, testing.isOperator(5, input));
    }
    /*
    @Test
    public void scientificNotationTest(){
        InflixToPostfix testing = new InflixToPostfix();

        Assertions.assertEquals("4.9E8", testing.scientificNotation("490000000"));
        Assertions.assertEquals("1.23E9", testing.scientificNotation("1230000000"));
        Assertions.assertEquals("5.05E7", testing.scientificNotation("50500000" ));
        Assertions.assertEquals("6.02E23", testing.scientificNotation("602000000000000000000000"  ));
        Assertions.assertEquals("8.0E20", testing.scientificNotation("800000000000000000000" ));
    }
     */
}