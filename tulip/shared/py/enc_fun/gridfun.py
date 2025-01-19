import math
import tulip

"""
methods having to do with a grid
e.g. drum grid, piano roll

"""
               
class Grid():
    def __init__(self, width, height, cols, rows, start_x, start_y, palette_index):
        self.width = width
        self.height = height
        self.cols = cols
        self.rows = rows
        self.start_x = start_x
        self.start_y = start_y
        self.palette_index = palette_index

        # compute grid spacing
        self.VerticalSpacing = self.height/self.rows
        self.HorizontalSpacing = self.width/self.cols

        #self.VerticalSpacing = math.floor(self.VerticalSpacing)
        #self.HorizontalSpacing = math.floor(self.HorizontalSpacing)


    def draw(self):

        print(f'VerticalSpacing: {self.VerticalSpacing}\n')
        print(f'HorizontalSpacing: {self.HorizontalSpacing}\n')
    
        # draw vertical lines
        for col in range(0, self.width-1, math.floor(self.HorizontalSpacing)):
            x0 = col + self.start_x
            x1 = col + self.start_x
            y0 = 0             + self.start_y
            y1 = self.height-1 + self.start_y
            tulip.bg_line(x0,y0,x1,y1,self.palette_index)

        # draw horizontal lines
        for row in range(0, self.height-1, math.floor(self.VerticalSpacing)):
            x0 = 0 + self.start_x
            x1 = self.width-1 + self.start_x
            y0 = row + self.start_y
            y1 = row + self.start_y
            tulip.bg_line(x0,y0,x1,y1,self.palette_index)


    def specs_get(self):
        return self.start_x, self.start_y, self.HorizontalSpacing, self.VerticalSpacing
    