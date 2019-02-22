public class AdvancedManipulations {

    /**
     * Change the contrast of a picture.
     *
     * Your job is to change the intensity of the colors in the picture.
     * The simplest method of changing contrast is as follows:
     *
     * 1. Find the average color intensity of the picture.
     *    a) Sum the values of all the color components for each pixel.
     *    b) Divide the total by the number of pixels times the number of
     *       components (3).
     * 2. Subtract the average color intensity from each color component of
     *    each pixel. This will make the average color intensity zero.
     * 3. Scale the intensity of each pixel's color components by multiplying
     *    them by the "multiplier" parameter. Note that the multiplier is a
     *    double (a decimal value like 1.2 or 0.6) and color values are ints
     *    between 0 and 255.
     * 4. Add the original average color intensity back to each component of
     *    each pixel.
     * 5. Clip the color values so that all color component values are between
     *    0 and 255. (This should be handled by the Pixel class anyway!)
     *
     * Hint: You should use Math.round() before casting to an int for the average
     * color intensity and for the scaled RGB values.
     */
    public static PixelPicture adjustContrast(
            PixelPicture pic, double multiplier) {
           int w = pic.getWidth();
           int h = pic.getHeight();
           int pixels = w * h;
           int sum = 0;
           

           Pixel[][] bmp = pic.getBitmap();
      
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                Pixel p = bmp[x][y];
                int r = p.getRed();
                int g = p.getGreen();
                int b = p.getBlue();
                
                sum = sum + (r + g + b);
            }
        }
        
     int colorIntensity = (int) (sum / (pixels * 3));
      
      for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                Pixel p = bmp[x][y];
                int r = p.getRed();
                int g = p.getGreen();
                int b = p.getBlue();
                int cR = (int) ((r - colorIntensity) * multiplier) + colorIntensity;
                int cG = (int) ((g - colorIntensity) * multiplier) + colorIntensity;
                int cB = (int) ((b - colorIntensity) * multiplier) + colorIntensity;
                bmp[x][y] = new Pixel(cR, cG, cB);
            }
        }
      
        return new PixelPicture(bmp);
        
    }

    /**
     * Reduce a picture to its most common colors.
     *
     * You will need to make use of the ColorMap class to generate a map from
     * Pixels of a certain color to the frequency with which identically-colored
     * pixels appear in the image. Once you have generated your ColorMap, select
     * your palette by retrieving the pixels whose color appears in the picture
     * with the highest frequency. Then change each pixel in the picture to one
     * with the closest matching color from your palette.
     *
     * Note that if there are two different colors that are the *same* minimal
     * distance from the given color, your code should select the most
     * frequently appearing one as the new color for the pixel. If both colors
     * appear with the same frequency, your code should select the one that
     * appears *first* in the output of the colormap's getSortedPixels.
     *
     * Algorithms like this are widely used in image compression. GIFs in
     * particular compress the palette to no more than 255 colors. The variant
     * we have implemented here is a weak one, since it only counts color
     * frequency by exact match. Advanced palette reduction algorithms (known as
     * "indexing" algorithms) calculate color regions and distribute the palette
     * over the regions. For example, if our picture had a lot of shades of blue
     * and a little bit of red, our algorithm would likely choose a palette of
     * all blue colors. An advanced algorithm would recognize that blues look
     * similar and distribute the palette so that it would be possible to
     * display red as well.
     *
     * @param pic The Pixel for which the closest match should be found.
     * @return The most closely matched Pixel from the palette.
     */
    public static PixelPicture reducePalette(PixelPicture pic, int numColors) {
      
       ColorMap c = new ColorMap();
       Pixel[][] bmp = pic.getBitmap();
       int w = pic.getWidth();
       int h = pic.getHeight();
      
       int v = 0;
       for(int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                Pixel p = bmp[x][y];
                v = 1;
              
                if(c.contains(p)){
                  v = v + c.getValue(p);
                }
              
                c.put(p,v);
            }
        }
      
       Pixel [] mapArray = c.getSortedPixels(); 
       Pixel [] ourPalette = new Pixel[numColors];
      
        for(int color = 0; color < numColors; color++){
          ourPalette[color] = mapArray[color];
        }
      
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
              
              Pixel p = bmp[x][y];  
              
              int dist = 0;
              int minDist = 1000000;
              int idx = numColors;
              
              for(int i = 0; i < numColors; i++){
                Pixel pr = ourPalette[i];
                dist = p.distance(pr);
                
                if(dist < minDist){
                  minDist = dist;
                  idx = i;
                } 
              }
              bmp[x][y] = ourPalette[idx];
            }
        }
       
        return new PixelPicture(bmp);
    }

    

    /**
     * Blur an image.
     *
     * There are different blurring algorithms, but we'll use the simplest:
     * box blur. Box blurring works by averaging the box-shaped neighborhood
     * around a pixel. The size of the box is configurable by setting the
     * radius, half the length of a side of the box. In the simplest case - a
     * radius of 1 - blurring just takes the average around a pixel. For
     * example, to blur around the pixel at (1,1) with radius 1, we take the
     * average value of the pixels of its neighborhood: (0,0) though (2,2),
     * including (1,1).
     *
     * NOTE: when blurring a pixel, make sure that you access the pixels in
     * its neighborhood in the original image. You shouldn't use already
     * "blurred" pixels to calculate the blur.
     *
     * Each Pixel is composed of 3 colors (red, green, and blue, or RGB) called
     * components. Each component should be averaged separately. For a radius
     * R, we set component c at location (x,y) according to the formula below:
     *
     * c = sum(c of each pixel within radius R) / ((2R + 1) * (2R + 1))
     *
     * Logically, it makes sense that this would be the formula for the
     * average. We sum the amount of each color in neighboring pixels and then
     * divide by the number of pixels we considered in the neighborhood.
     *
     * Note that this equation disregards corner cases. When blurring (0,0)
     * with radius 1, we only need to consider the top-left corner, (0,0)
     * through (1,1) - we'll divide by 4 at the end, not 9. You'll have to be
     * careful to only access bitmaps inside of their bounds.
     *
     * You can assume that you will not be given a radius less than 1.
     *
     * Note: In computing the average, you should be doing double division.
     * Use Math.round() before casting to an int when computing the average.
     * If you do not follow these specifications, the values you compute will
     * not pass our tests.
     *
     * @param pic The picture to be blurred.
     * @param radius The radius of the blurring box.
     * @return A blurred version of the original picture.
     */
  
    public static Pixel blurPixel(Pixel[][] src,int x, int y, int radius) {
      
        int w = src.length;
        int h = src[0].length;
      
        int startX = Math.max((x - radius), 0);
        int startY = Math.max((y - radius), 0);
        int endX = Math.min((x + radius + 1), w);
        int endY = Math.min((y + radius + 1), h);
        int entryR = 0;
        int entryG = 0;
        int entryB = 0;
        double count = 0.0;
      
      
        for (int i = startX; i < endX; i++) {
            for (int j = startY; j < endY; j++) {
  
                Pixel p = src[i][j];
                int r = p.getRed();
                int g = p.getGreen();
                int b = p.getBlue();
    
                entryR = entryR + r;
                entryG = entryG + g;
                entryB = entryB + b;
                count = count + 1.0;
              
                }
            }
   
        entryR = (int) Math.round(entryR / count);
        entryG = (int) Math.round(entryG / count);
        entryB = (int) Math.round(entryB / count);
      
        Pixel blurPixel = new Pixel(entryR, entryG, entryB);
      
        return blurPixel;
        }
  
    public static PixelPicture blur(PixelPicture pic, int radius) {
        int w = pic.getWidth();
        int h = pic.getHeight();
      
        Pixel[][] src = pic.getBitmap();
        Pixel[][] tgt = new Pixel[w][h];
      
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {                
                tgt[x][y] = blurPixel(src, x, y, radius);
               }
            }
      
        return new PixelPicture(tgt);
    }
 

    // You may want to add a static helper function here to help find the
    // average color around the pixel you are blurring.

    /**
     * Challenge Problem (this problem is worth 0 points):
     * Flood pixels of the same color with a different color.
     *
     * The name is short for flood fill, which is the familiar "paint bucket"
     * operation in graphics programs. In a paint program, the user clicks on a
     * point in the image. Every neighboring, similarly-colored point is then
     * "flooded" with the color the user selected.
     *
     * Suppose we want to flood color at (x,y). The simplest way to do flood
     * fill is as follows:
     *
     * 1. Let target be the color at (x,y).
     * 2. Create a set of points Q containing just the point (x,y).
     * 3. Take the first point p out of Q.
     * 4. Set the color at p to color.
     * 5. For each of p's non-diagonal neighbors - up, down, left, and right -
     *    check to see if they have the same color as target. If they do, add
     *    them to Q.
     * 6. If Q is empty, stop. Otherwise, go to 3.
     *
     * This is a naive algorithm that can be made significantly faster if you
     * wish to try.
     *
     * For Q, you should use the provided IntQueue class. It works very much
     * like the queues we implemented in OCaml.
     *
     * @param pic The original picture to be flooded.
     * @param c The pixel the user "clicked" (representing the color that should be flooded).
     * @param x The x-coordinate of the point on which the user "clicked."
     * @param y The y-coordinate of the point on which the user "clicked."
     * @return A new picture with the appropriate region flooded.
     */
    public static PixelPicture flood(PixelPicture pic, Pixel c, int x, int y) {
        return pic;
    }
}
