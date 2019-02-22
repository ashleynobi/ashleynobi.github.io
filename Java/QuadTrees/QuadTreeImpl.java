
public class QuadTreeImpl implements QuadTree {

	QuadNode root;

	public QuadTreeImpl(int[][] img) {
		int mid = (img.length / 2);
		root = new QuadNodeImpl(img.length);

		assign(root, 0, 0, mid, img);
	}

	void assign(QuadNode node, int x, int y, int mid, int[][] src) {
		//assigns nodes based on array values
		boolean isLeaf = true;
		int hiX = x + mid;
		int hiY = y + mid;

		if ((src[y][x] != src[hiY][x] 
			|| src[hiY][x] != src[y][hiX] 
			|| src[y][hiX] != src[hiY][hiX]) 
			&& hiY != y && hiX != x) {
			
			isLeaf = false;
		} else {
				for (int i = 0; (i < mid) && isLeaf; i++) {
					for (int j = 0; (j < mid) && isLeaf; j++) {
						if (src[y][x] != src[y + i][x + j] 
								|| src[hiY][x] != src[hiY + i][x + j]
								|| src[y][hiX] != src[y + i][hiX + j] 
								|| src[hiY][hiX] != src[hiY + i][hiX + j]) {
							isLeaf = false;
						}
					}
				}
		}
		
		
		if (isLeaf) {
			node.setColor(src[y][x]);

		} else {
			QuadNode tL = new QuadNodeImpl(mid);
			QuadNode tR = new QuadNodeImpl(mid);
			QuadNode bL = new QuadNodeImpl(mid);
			QuadNode bR = new QuadNodeImpl(mid);
			
			node.setQuadrant(QuadName.TOP_LEFT, tL);
			node.setQuadrant(QuadName.TOP_RIGHT, tR);
			node.setQuadrant(QuadName.BOTTOM_LEFT, bL);
			node.setQuadrant(QuadName.BOTTOM_RIGHT, bR);
			
			mid = (tL.getDimension() / 2);
			assign(tL, x, y, mid, src);
			assign(tR, hiX, y, mid, src);
			assign(bL, x, hiY, mid, src);
			assign(bR, hiX, hiY, mid, src);
		}
	}
	
	QuadNode findNode(int x, int y, int midX, int midY, QuadNode toGet){
		//for both get and set methods, finds leaf closest to pixel in array
		if(toGet.isLeaf()) {
			return toGet;
			
		} else {
			QuadName name = QuadName.TOP_LEFT;
			
			if (x < midX && y < midY) {
				name = QuadName.TOP_LEFT;
				midX = midX - (toGet.getDimension() / 2);
				midY = midY - (toGet.getDimension() / 2);
			}

			if (x >= midX && y < midY) {
				name = QuadName.TOP_RIGHT;
				midX = midX + (toGet.getDimension() / 2);
				midY = midY - (toGet.getDimension() / 2);
			}

			if (x < midX && y >= midY) {
				name = QuadName.BOTTOM_LEFT;
				midX = midX - (toGet.getDimension() / 2);
				midY = midY + (toGet.getDimension() / 2);
			}	

			if (x >= midX && y >= midY) {
				name = QuadName.BOTTOM_RIGHT;
				midX = midX + (toGet.getDimension() / 2);
				midY = midY + (toGet.getDimension() / 2);
			}
		
			return findNode(x, y, midX, midY, toGet.getQuadrant(name));
		} 
}
	
	QuadNode createChildren(int x, int y, int midX, int midY, QuadNode leaf){
			//restructures array and returns QuadNode of dim 1 corresponding to pixel
			//in array
			if(leaf.getDimension() == 1){
				return leaf;
			} else{
				
			QuadName name = QuadName.TOP_LEFT;
			
			QuadNode tL = new QuadNodeImpl(leaf.getDimension()/2);
			QuadNode tR = new QuadNodeImpl(leaf.getDimension()/2);
			QuadNode bL = new QuadNodeImpl(leaf.getDimension()/2);
			QuadNode bR = new QuadNodeImpl(leaf.getDimension()/2);
			
			leaf.setQuadrant(QuadName.TOP_LEFT, tL);
			leaf.setQuadrant(QuadName.TOP_RIGHT, tR);
			leaf.setQuadrant(QuadName.BOTTOM_LEFT, bL);
			leaf.setQuadrant(QuadName.BOTTOM_RIGHT, bR);
			
			tL.setColor(leaf.getColor());
			tR.setColor(leaf.getColor());
			bL.setColor(leaf.getColor());
			bR.setColor(leaf.getColor());
			
			leaf.setColor(-1);
			
			if (x < midX && y < midY) {
				name = QuadName.TOP_LEFT;
				midX = (leaf.getDimension() / 2);
				midY = (leaf.getDimension() / 2);
			}

			if (x >= midX && y < midY) {
				name = QuadName.TOP_RIGHT;
				midX = midX + (leaf.getDimension() / 2);
				midY = (leaf.getDimension() / 2);
			}

			if (x < midX && y >= midY) {
				name = QuadName.BOTTOM_LEFT;
				midX = (leaf.getDimension() / 2);
				midY = midY + (leaf.getDimension() / 2);
			}

			if (x >= midX && y >= midY) {
				name = QuadName.BOTTOM_RIGHT;
				midX = midX + (leaf.getDimension() / 2);
				midY = midY + (leaf.getDimension() / 2);
			}
			
			return createChildren(x, y, midX, midY, leaf.getQuadrant(name));
		}
	}

	/**
	 * Sets the color at coordinates {@code (x, y)}.
	 *
	 * @param x
	 *            the {@code x}-coordinate
	 * @param y
	 *            the {@code y}-coordinate
	 * @param color
	 *            the color
	 * @throws IllegalArgumentException
	 *             if either {@code x} or {@code y} is out of bounds
	 */
	public void setColor(int x, int y, int color) {
		if (x > this.getDimension() || y > this.getDimension() 
			|| x < 0 || y < 0) {
			throw new IllegalArgumentException();
		}

		int midX = (this.getDimension() / 2);
		int midY = (this.getDimension() / 2);
		QuadNode toSet = this.root;

		QuadNode curr = findNode(x, y, midX, midY, toSet);
		
		if(curr.getColor() != color){
			QuadNode ourPixel = createChildren(x, y, midX, midY, curr);
			ourPixel.setColor(color);
		}
	}

	/**
	 * Gets the color at coordinates {@code (x, y)}.
	 *
	 * @param x
	 *            the {@code x}-coordinate
	 * @param y
	 *            the {@code y}-coordinate
	 * @return the color
	 * @throws IllegalArgumentException
	 *             if x, y are out of bounds
	 */
		
	
	public int getColor(int x, int y) {
		if (x > this.getDimension() || y > this.getDimension() || x < 0 || y < 0) {
			throw new IllegalArgumentException();
		}

		int midX = (this.getDimension() / 2);
		int midY = (this.getDimension() / 2);
		QuadNode toGet = this.root;

		return (findNode(x,y,midX,midY,toGet)).getColor();

	}

	/**
	 * Gets the dimension of the image encoded by this {@link QuadTree}. This is the
	 * size of the side of the square of pixels covered by the root
	 * {@link QuadNode}.
	 *
	 * @return a non-negative integer power of two
	 */
	public int getDimension() {
		return root.getDimension();
	}

	void fill(int[][] src, int x, int y, QuadNode node) {
		// fills array components based on tree structure
		if (node.isLeaf()) {
			int dim = node.getDimension();

			for (int i = x; i < x + dim; i++) {
				for (int j = y; j < y + dim; j++) {
					src[j][i] = node.getColor();
				}
			}
		} else {
			QuadNode tL = node.getQuadrant(QuadName.TOP_LEFT);
			QuadNode tR = node.getQuadrant(QuadName.TOP_RIGHT);
			QuadNode bL = node.getQuadrant(QuadName.BOTTOM_LEFT);
			QuadNode bR = node.getQuadrant(QuadName.BOTTOM_RIGHT);

			int mid = (node.getDimension() / 2);

			fill(src, x, y, tL);
			fill(src, x + mid, y, tR);
			fill(src, x, y + mid, bL);
			fill(src, x + mid, y + mid, bR);

		}
	}

	/**
	 * Decompresses the {@link QuadTree} into a flat image. The returned array
	 * contains integers that represent the color at each coordinate. The returned
	 * 2D array satisfies {@code
	 * result[y][x] == getColor(x, y)}.
	 * <p/>
	 * You should implement this method as efficiently as possible.
	 *
	 * @return a newly initialized array storing the decompressed image data
	 */
	public int[][] decompress() {
		int[][] tgt = new int[this.getDimension()][this.getDimension()];
		QuadNode mother = root;
		
		fill(tgt, 0, 0, mother);
		
		return tgt;
	}

	/**
	 * Gets the compression ratio of the current {@link QuadTree}. The compression
	 * ratio is defined as the number of {@link QuadNode}s contained in the tree,
	 * divided by the number of pixels in the image.
	 *
	 * @return the compression ratio as a positive double
	 */
	public double getCompressionRatio() {
		double pixels = this.getDimension() * this.getDimension();
		double nodes = (this.root).getSize();

		return (nodes / pixels);
	}

	/**
	 * Returns the root node of the {@link QuadTree} for testing.
	 * <p/>
	 * You may assume that outside code will not modify the {@link QuadNode}
	 * returned by this method.
	 *
	 * @return root QuadNode
	 */
	public QuadNode getRoot() {
		return root;
	}
}
