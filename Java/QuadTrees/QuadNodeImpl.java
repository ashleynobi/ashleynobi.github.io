
public class QuadNodeImpl implements QuadNode {

	private QuadNode topleft;
	private QuadNode topright;
	private QuadNode bottomleft;
	private QuadNode bottomright;
	private int color;
	private int dim;

	public QuadNodeImpl(int dim) {
		topleft = null;
		topright = null;
		bottomleft = null;
		bottomright = null;
		color = -1;
		this.dim = dim;
	}

	public int getColor() {
		return this.color;
	}

	public void setColor(int newColor) {
		this.color = newColor;
	}

	public QuadNode getQuadrant(QuadName quadrant) {

		switch (quadrant) {
		case TOP_LEFT:
			return this.topleft;
		case TOP_RIGHT:
			return this.topright;
		case BOTTOM_LEFT:
			return this.bottomleft;
		case BOTTOM_RIGHT:
			return this.bottomright;
		default:
			return null;
		}
	}

	public boolean isLeaf() {

		boolean nochildren = (this.topleft == null && this.topright == null 
				              && this.bottomleft == null && this.bottomright == null);

		boolean hascolor = (this.color != -1);

		return (nochildren && hascolor);
	}

	public int getDimension() {
		return dim;
	}

	public int getSize() {
		if (isLeaf()) {
			return 1;
		} else{
			return 1 + topleft.getSize() + topright.getSize() + bottomleft.getSize() + bottomright.getSize();
		}
	}

	public void setQuadrant(QuadName quadrant, QuadNode value) {
		switch (quadrant) {
		case TOP_LEFT:
			this.topleft = value;
		case TOP_RIGHT:
			this.topright = value;
		case BOTTOM_LEFT:
			this.bottomleft = value;
		case BOTTOM_RIGHT:
			this.bottomright = value;
		}
	}
}
