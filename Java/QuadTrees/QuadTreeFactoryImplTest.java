import static org.junit.Assert.*;

import org.junit.Test;

public class QuadTreeFactoryImplTest {

	int[][] regularArr = {{1,2},{3,4}};
	int[][] nullArr = null;
	int[][] zeroArr = {{}};
	int[][] unEvenArr = {{1},{2,3}};
	int[][] rectangleArr = {{1,2,3},{4,5,6}};
	int[][] notPowOfTwoArr = {{1,2,3},{4,5,6},{7,8,9}};
	QuadTreeFactory factory = new QuadTreeFactoryImpl();

	
	@Test(expected = IllegalArgumentException.class)
	public void notInitializedNull() {
		QuadTree nullTree = factory.buildFromIntArray(nullArr);
	}
	
	@Test(expected = IllegalArgumentException.class)
	public void notInitializedZero() {
		QuadTree zeroTree = factory.buildFromIntArray(zeroArr);
	}
	
	@Test(expected = IllegalArgumentException.class)
	public void notInitializedUneven() {
		QuadTree unevenTree = factory.buildFromIntArray(unEvenArr);
	}
	
	@Test(expected = IllegalArgumentException.class)
	public void notInitializedRectangle() {
		QuadTree rectangleTree = factory.buildFromIntArray(rectangleArr);
	}
	
	@Test(expected = IllegalArgumentException.class)
	public void notInitializedNotPowOfTwo() {
		QuadTree notPowTree = factory.buildFromIntArray(notPowOfTwoArr);
	}
	
	@Test
	public void initialized() {
		QuadTree regularTree = factory.buildFromIntArray(regularArr);
		assertEquals(regularTree.getDimension(), 2);
	}
	
	

}
