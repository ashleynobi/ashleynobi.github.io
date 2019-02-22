import org.junit.Test;
import static org.junit.Assert.*;


/** 
 *  Use this file to test your implementation of Pixel.
 * 
 *  We will manually grade this file to give you feedback
 *  about the completeness of your test cases.
 */

public class MyPixelTest {

    /*
     * Remember, UNIT tests should ideally have one point of failure. Below we
     * give you an example of a unit test for the Pixel constructor that takes
     * in three ints as arguments. We use the getRed(), getGreen(), and
     * getBlue() methods to check that the values were set correctly. This one
     * test does not comprehensively test all of Pixel so you must add your own.
     *
     * You might want to look into assertEquals, assertTrue, assertFalse, and
     * assertArrayEquals at the following:
     * http://junit.sourceforge.net/javadoc/org/junit/Assert.html
     *
     * Note, if you want to add global variables so that you can reuse Pixels
     * in multiple tests, feel free to do so.
     */

    @Test
    public void testConstructInBounds() {
        Pixel p = new Pixel(40, 50, 60);
        assertEquals(40, p.getRed());
        assertEquals(50, p.getGreen());
        assertEquals(60, p.getBlue());
       
    }

    /* ADD YOUR OWN TESTS BELOW */
  
    @Test
    public void testConstructOutofBoundsOver() {
        Pixel p = new Pixel(40, 50, 300);
        assertEquals(40, p.getRed());
        assertEquals(50, p.getGreen());
        assertEquals(255, p.getBlue());
        
    }
  
    @Test
    public void testConstructOutofBoundsUnder() {
        Pixel p = new Pixel(40, -50, 60);
        assertEquals(40, p.getRed());
        assertEquals(0, p.getGreen());
        assertEquals(60, p.getBlue());
    }
    
    @Test
    public void testConstructCorrectArray() {
        int [] c = {40, 50, 60};
        Pixel p = new Pixel(c);
        assertEquals(40, p.getRed());
        assertEquals(50, p.getGreen());
        assertEquals(60, p.getBlue());
    }
  
    @Test
    public void testConstructArrayTooLong() {
        int [] c = {40, 50, 60, 70, 80};
        Pixel p = new Pixel(c);
        assertEquals(40, p.getRed());
        assertEquals(50, p.getGreen());
        assertEquals(60, p.getBlue());
    }
  
    @Test
    public void testConstructArrayTooShort() {
        int [] c = {40};
        Pixel p = new Pixel(c);
        assertEquals(40, p.getRed());
        assertEquals(0, p.getGreen());
        assertEquals(0, p.getBlue());
    }
  
    @Test
    public void testGetComponentsNormal() {
        Pixel p = new Pixel(40, 50, 60);
        int [] a = {40, 50, 60};
        assertArrayEquals(a, p.getComponents());
    }
  
    @Test
    public void testGetComponentsArray() {
        int [] a = {40, 50, 60};
        Pixel p = new Pixel(a);
        assertArrayEquals(a, p.getComponents());
    }
  
    @Test
    public void testDistanceOneDifference() {
        Pixel p = new Pixel(40, 50, 60);
        Pixel px = new Pixel(70, 50, 60);
        assertEquals(30, p.distance(px));
    }
  
    @Test
    public void testDistanceAllDifferent() {
        Pixel p = new Pixel(40, 50, 60);
        Pixel px = new Pixel(70, 70, 70);
        assertEquals(60, p.distance(px));
    }
  
    @Test
    public void testToString() {
        Pixel p = new Pixel(40, 50, 60);
        assertEquals("(40, 50, 60)", p.toString());
    }
  
    @Test
    public void testEqualsIsEqual() {
        Pixel p = new Pixel(40, 50, 60);
        Pixel px = new Pixel(40, 50, 60);
        assertEquals(true, p.equals(px));
    }
  
    @Test
    public void testEqualsNotEqual() {
        Pixel p = new Pixel(40, 50, 60);
        Pixel px = new Pixel(40, 50, 80);
        assertEquals(false, p.equals(px));
    }
  
    
}
