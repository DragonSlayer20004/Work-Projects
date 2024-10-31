import org.junit.jupiter.api.*;

public class ListTest {

    private final NewList list = new NewList();

    @BeforeEach
    public void addElements() {
        list.add("Every");
        list.add("Good");
        list.add("Boy");
        list.add("Deserves");
    }

    @Test
    public void testAddElementsSpecific() {
        list.add(2, "Dog");
        Assertions.assertEquals("Dog", list.get(2));
        Assertions.assertEquals("Boy", list.get(3));
    }

    @Test
    public void testGet() {
        Assertions.assertEquals("Good", list.get(1));
    }

    @Test
    public void testGetException() {
        try {
            String value = list.get(4);
            Assertions.fail("An exception should have been thrown");
        }
        catch (IndexOutOfBoundsException e) {}
    }

    @Test
    public void testAddAll() {
        NewList list2 = new NewList();
        list2.add("I");
        list2.add("Believe");

        list2.addAll(list);

        Assertions.assertEquals("Boy", list2.get(4));
    }

    @Test
    public void testEmpty() {
        Assertions.assertFalse(list.isEmpty());

        list.clear();

        Assertions.assertTrue(list.isEmpty());
    }

    @Test
    public void testSize() {
        Assertions.assertEquals(4, list.size());
    }


    @Test
    public void testIndexOf() {
        list.remove(1);

        //noinspection StringOperationCanBeSimplified
        Assertions.assertEquals(2, list.indexOf(new String("Deserves")));
    }

    @Test
    public void testLastIndexOf(){
        list.add("Treats");
        list.add("And");
        list.add("Pets");
        list.add("Boy");
        Assertions.assertEquals(7, list.lastIndexOf("Boy"));
    }

    @Test
    public void testContains() {
        Assertions.assertEquals(true, list.contains("Good"));
        Assertions.assertEquals(false, list.contains("Bob"));
    }

    @Test
    public void testSet() {
        list.set(2, "Dog");
        Assertions.assertEquals("Dog", list.get(2));
        try {
            String value = list.get(8);
            Assertions.fail("An exception should have been thrown");
        }
        catch (IndexOutOfBoundsException e) {}

    }
}