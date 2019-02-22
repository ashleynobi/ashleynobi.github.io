import static org.junit.Assert.*;

import java.util.Arrays;

import org.junit.Before;

import org.junit.Test;

public class QuadTreeImplTest {
	
	//test/given arrays
	int smallImg[][] = {{1,1,0,0},
						{1,1,0,1},
						{0,0,1,1},
						{0,1,1,1}};
	
	int [] img1 = {1,1,1,1,0,0,0,0};
	int [] img2 = {1,1,1,1,0,0,0,0};
	int [] img3 = {1,1,1,1,0,0,0,0};
	int [] img4 = {1,1,1,1,0,0,0,0};
	int [] img5 = {1,1,0,0,0,1,0,1};
	int [] img6 = {1,1,0,0,1,0,1,0};
	int [] img7 = {0,0,1,1,0,1,0,1};
	int [] img8 = {0,0,1,1,1,0,1,0};
	int img[][] = {img1,img2,img3,img4,img5,img6,img7,img8};
	
	QuadTree smallTree;
	QuadTree tree;
	
	@Before
	public void createTree(){
		smallTree = new QuadTreeImpl(smallImg);
		tree = new QuadTreeImpl(img);
	}

	@Test(expected = IllegalArgumentException.class)
	public void testSetColorFails() {
		//testing for incorrect edge case
		smallTree.setColor(5, 3, 2);
	}
	

	@Test
	(expected = IllegalArgumentException.class)
	public void testGetColorFails() {
		//testing for incorrect edge case
		tree.getColor(3, 9);
	}
	
	@Test
	public void testGetandSetColor() {
		//testing get
		assertEquals(smallTree.getColor(1, 3), 1);
		assertEquals(smallTree.getColor(0, 0), 1);
		
		//original tree properties
		QuadNode root1 = smallTree.getRoot();
		assertEquals(root1.getSize(), 13);
		
		//testing changes from set
		smallTree.setColor(1,3,2);
		assertEquals(root1.getSize(), 13);
		assertEquals(smallTree.getColor(1, 3), 2);
		
		smallTree.setColor(0,0,2);
		assertEquals(root1.getSize(), 17);
		assertEquals(smallTree.getColor(0, 0), 2);
		
	    
		//same process on larger tree
		assertEquals(tree.getColor(4, 7), 1);
		assertEquals(tree.getColor(4, 1), 0);
		
		QuadNode root2 = tree.getRoot();
		assertEquals(root2.getSize(), 29);
		
		tree.setColor(4,7,2);
		assertEquals(root2.getSize(), 29);
		assertEquals(tree.getColor(4, 7), 2);
		
		tree.setColor(4,1,2);
		assertEquals(root2.getSize(), 33);
		assertEquals(tree.getColor(4, 1), 2);
		
	}
	
	
	@Test
	public void testGetDimension() {
		//compares getDimension to known length of given arrays
		assertEquals(smallTree.getDimension(), 4);
		assertEquals(tree.getDimension(), 8);
	}
	
	
	@Test
	public void testDecompress() {
		// creates arrays using decompress and compares them to test arrays
		int[][] smallTreeDecompress = smallTree.decompress();
		int[][] treeDecompress = tree.decompress();
		Arrays.equals(smallImg,smallTreeDecompress);
		Arrays.equals(img,treeDecompress);
	}

	@Test
	public void testGetCompressionRatio() {
		//tests based on amount of pixels (known from test arrays)
		assertTrue((smallTree.getCompressionRatio()) * 16 == 13);
		assertTrue(tree.getCompressionRatio() * 64 == 29);
	}
	
	@Test
	public void testGetRoot() {
		//gets roots and determines validity by root qualities/children
		QuadNode root1 = smallTree.getRoot();
		QuadNode root2 = tree.getRoot();
		assertEquals(root1.getColor(), -1);
		assertEquals(root2.getColor(), -1);
		assertEquals(root1.getSize(), 13);
		assertEquals(root2.getSize(), 29);
		
		QuadNode leaf = root1.getQuadrant(QuadName.TOP_LEFT);
		QuadNode notLeaf = root2.getQuadrant(QuadName.BOTTOM_RIGHT);
		
		assertTrue(leaf.isLeaf());
		assertFalse(notLeaf.isLeaf());
				
	}
	


}
