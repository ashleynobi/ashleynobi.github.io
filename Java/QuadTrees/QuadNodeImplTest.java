import static org.junit.Assert.*;
import org.junit.Before;

import org.junit.Test;

public class QuadNodeImplTest {
	
	QuadNode leaf;
	QuadNode mother;
	QuadNode grandmother;
	QuadNode matriarch;
	
	@Before
	public void createNodes(){
		leaf = new QuadNodeImpl(4);
		QuadNode leaf2 = new QuadNodeImpl(4);
		QuadNode leaf3 = new QuadNodeImpl(4);
		QuadNode leaf4 = new QuadNodeImpl(4);
		QuadNode leaf5 = new QuadNodeImpl(8);
		QuadNode leaf6 = new QuadNodeImpl(8);
		QuadNode leaf7 = new QuadNodeImpl(16);
		QuadNode leaf8 = new QuadNodeImpl(16);
		QuadNode leaf9 = new QuadNodeImpl(16);
		
		leaf.setColor(1);
		leaf2.setColor(2);
		leaf3.setColor(3);
		leaf4.setColor(4);
		leaf5.setColor(5);
		leaf6.setColor(6);
		leaf7.setColor(7);
		leaf8.setColor(8);
		leaf9.setColor(9);
		
		
		mother = new QuadNodeImpl(8);
		mother.setQuadrant(QuadName.TOP_LEFT, leaf);
		mother.setQuadrant(QuadName.TOP_RIGHT, leaf2);
		mother.setQuadrant(QuadName.BOTTOM_LEFT, leaf3);
		mother.setQuadrant(QuadName.BOTTOM_RIGHT, leaf4);
		
		QuadNode mother2 = new QuadNodeImpl(8);
		mother2.setQuadrant(QuadName.TOP_LEFT, leaf4);
		mother2.setQuadrant(QuadName.TOP_RIGHT, leaf3);
		mother2.setQuadrant(QuadName.BOTTOM_LEFT, leaf2);
		mother2.setQuadrant(QuadName.BOTTOM_RIGHT, leaf);
		
		grandmother = new QuadNodeImpl(16);
		grandmother.setQuadrant(QuadName.TOP_LEFT, leaf5);
		grandmother.setQuadrant(QuadName.TOP_RIGHT, mother);
		grandmother.setQuadrant(QuadName.BOTTOM_LEFT, leaf6);
		grandmother.setQuadrant(QuadName.BOTTOM_RIGHT, mother2);
		
		matriarch = new QuadNodeImpl(32);
		matriarch.setQuadrant(QuadName.TOP_LEFT, leaf7);
		matriarch.setQuadrant(QuadName.TOP_RIGHT, leaf8);
		matriarch.setQuadrant(QuadName.BOTTOM_LEFT, grandmother);
		matriarch.setQuadrant(QuadName.BOTTOM_RIGHT, leaf9);
	}
	

	@Test
	public void testGetColor() {
		assertEquals(leaf.getColor(), 1);
		assertEquals(mother.getColor(), -1);
		assertEquals(grandmother.getColor(), -1);
		assertEquals(matriarch.getColor(), -1);
	}
	
	@Test
	public void testSetColor() {
		assertEquals(leaf.getColor(), 1);
		leaf.setColor(22);
		assertEquals(leaf.getColor(), 22);
	}
	
	
	@Test
	    public void testGetQuadrant() {
		QuadNode check1 = mother.getQuadrant(QuadName.TOP_LEFT);
		QuadNode check2 = grandmother.getQuadrant(QuadName.TOP_RIGHT);
		QuadNode check3 = matriarch.getQuadrant(QuadName.BOTTOM_LEFT);
		assertEquals(leaf,check1);
		assertEquals(mother,check2);
		assertEquals(grandmother,check3);
		
	}
	
	@Test
	public void testIsLeaf() {
		assertTrue(leaf.isLeaf());
		assertFalse(mother.isLeaf());
		
	}
	
	@Test
	public void testGetDimension() {
		assertEquals(leaf.getDimension(), 4);
		assertEquals(mother.getDimension(), 8);
		assertEquals(grandmother.getDimension(), 16);
		assertEquals(matriarch.getDimension(), 32);
		
	}
	
	@Test
	public void testGetSize() {
		 assertEquals(1,leaf.getSize());
		 assertEquals(5, mother.getSize());
		 assertEquals(13, grandmother.getSize());
		 assertEquals(17, matriarch.getSize());
		
	}
	
	
	@Test
	public void testSetQuadrant() {
		QuadNode check = mother.getQuadrant(QuadName.TOP_RIGHT);
		assertTrue(check != leaf);
		mother.setQuadrant(QuadName.TOP_RIGHT, leaf);
		check = mother.getQuadrant(QuadName.TOP_LEFT);
		assertEquals(check,leaf);
		
		
	}
	
	

}
