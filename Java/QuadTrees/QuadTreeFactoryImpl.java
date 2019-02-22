public class QuadTreeFactoryImpl implements QuadTreeFactory {

	@Override
	public QuadTree buildFromIntArray(int[][] imageData) {
		// TODO: implement build

		if (imageData == null || imageData.length == 0 || imageData[0].length == 0
				|| imageData.length != imageData[0].length) {
			throw new IllegalArgumentException();
		}

		for (int i = 0; i < imageData.length - 1; i++) {
			if (imageData[i].length != imageData[i + 1].length) {
				throw new IllegalArgumentException();
			}
		}

		int num = imageData.length;

		while (num > 1) {
			if (num % 2 == 0) {
				num = num / 2;
			} else {
				throw new IllegalArgumentException();
			}
		}

		return new QuadTreeImpl(imageData);
	}
}
