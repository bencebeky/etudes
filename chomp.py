#/usr/bin/env python
# coding=utf8
# Object-oriented 2d chomp implementation using a general poset game engine.

"""
Copyright 2013, 2014 Bence Béky

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
"""

import numpy;
import Tkinter;
import PIL.Image;
import PIL.ImageTk;

highlightfunction = lambda x: 0.8*x;

# The graphical layer: drawing and user interaction.
class ChompGraphics(Tkinter.Canvas):
  def __init__(self, parent):
    """Create graphical object, initialize internal variables, create game engine instance."""
    Tkinter.Canvas.__init__(self, parent);
    self.parent = parent;
    # load graphical representation of a single cell
    self.cellimage = PIL.Image.open("chomp-cell.png");
    self.cellphoto = PIL.ImageTk.PhotoImage(self.cellimage);
    self.cellphotohighlighted = PIL.ImageTk.PhotoImage(PIL.Image.eval(self.cellimage,highlightfunction));
    self.cellwidth = self.cellphoto.width()-1;
    self.cellheight = self.cellphoto.height()-1;
    # do not highlight move until mouse is moved
    self.havemouse = False;
    self.mousex = None;
    self.mousey = None;
    # initialize game engine
    self.ChompEngineInstance = ChompEngine();
    # board size at least 2x2, and computer starts, so player always loses
    self.parent.minsize(2*self.cellwidth, 2*self.cellheight);
    # this will trigger size.redraw
    self.resize(4, 3);
    self.pack();
    return;
  def resize(self, n=None, m=None):
    """Resize canvas to new window size or resize both to given size, restart game if table size changed."""
    # If called without an argument, take size of parent(window).
    if(n is None) |(m is None):
      width = self.parent.winfo_width();
      height = self.parent.winfo_height();
      newn = int(round(float(self.parent.winfo_width())/self.cellwidth));
      newm = int(round(float(self.parent.winfo_height())/self.cellheight));
      newwidth = newn*self.cellwidth;
      newheight = newm*self.cellheight;
      # If window needs to be resized to integer cell numbers, do it.
      # Note that this check is necessary to avoid an infinite loop,
      # because a parent window resize triggers this function again.
      if(width != newwidth) |(height != newheight):
        #print newwidth, newheight;
        self.parent.geometry("{0:d}x{1:d}".format(newwidth, newheight));
        self.configure(width=newwidth, height=newheight);
        # If table size changed, restart game.
        if(newn != self.n) |(newm != self.m):
          self.n = newn;
          self.m = newm;
          self.ChompEngineInstance.setsize(self.n, self.m);
        self.redraw();
    # If called with an argument, resize to that specified size.
    else:
      self.n = n;
      self.m = m;
      newwidth = n*self.cellwidth;
      newheight = m*self.cellheight;
      self.parent.geometry("{0:d}x{1:d}".format(newwidth, newheight));
      self.configure(width=newwidth, height=newheight);
      self.ChompEngineInstance.setsize(self.n, self.m);
      self.redraw();
    return;
  def redraw(self):
    """Redraw entire game board: should be called upon every resize, move, and mouse movement."""
    # clear canvas
    self.delete("all");
    # if we have the pointer, highlight potential move
    if self.havemouse &(self.mousex is not None) &(self.mousey is not None):
      visualarray = self.ChompEngineInstance.visualarray([int(numpy.floor(self.mousex / self.cellwidth)), int(self.m-1-numpy.floor(self.mousey / self.cellheight))]);
    else:
      visualarray = self.ChompEngineInstance.visualarray();
    for i in range(self.n):
      for j in range(self.m):
        if visualarray [i,j] == 1:
          self.create_image(i*self.cellwidth,(self.m-1-j)*self.cellheight, image=self.cellphotohighlighted, anchor='nw');
        elif visualarray [i,j] == 0:
          self.create_image(i*self.cellwidth,(self.m-1-j)*self.cellheight, image=self.cellphoto, anchor='nw');
        else:
          self.create_rectangle(i*self.cellwidth,(self.m-1-j)*self.cellheight,(i+1)*self.cellwidth,(self.m-j)*self.cellheight, fill='black');
    # if player lost the game, notify them in the title bar
    if self.ChompEngineInstance.lost():
      self.parent.title("You lost. Click or resize to restart.");
    else:
      self.parent.title("Chomp {0:d}x{1:d}".format(self.n, self.m));
    return;
  def leave(self):
    """Mouse leaves the window: take note."""
    self.havemouse = False;
    self.redraw();
    return;
  def motion(self, x, y):
    """Mouse moves in the window: redraw to highlight potential move."""
    self.havemouse = True;
    self.mousex = x;
    self.mousey = y;
    self.redraw();
    return;
  def click(self, x, y):
    """Mouse click in window: player makes a move."""
    self.havemouse = True;
    self.mousex = x;
    self.mousey = y;
    self.ChompEngineInstance.playermoves([int(numpy.floor(self.mousex / self.cellwidth)), int(self.m-1-numpy.floor(self.mousey / self.cellheight))]);
    self.redraw();
    return;

# functions for the graphical engine to be bound to window events
def bind_resize(event):
  ChompGraphicsInstance.resize();
  return;

def bind_leave(event):
  ChompGraphicsInstance.leave();
  return;

def bind_motion(event):
  ChompGraphicsInstance.motion(event.x, event.y);
  return;

def bind_click(event):
  ChompGraphicsInstance.click(event.x, event.y);
  return;

# the game engine: winning and losing positions, potential steps color-code
class ChompEngine():
  def __init__(self):
    """Initialize game engine."""
    return;
  def setsize(self, n, m):
    """Set board size and start game."""
    self.n = n;
    self.m = m;
    self.statelist = [];
    self.startgame();
    return;
  def startgame(self):
    """Start game: initialize state, let computer start."""
    self.currentstate = [];
    self.computermoves();
    return;
  def valid(self, step, state=None):
    """Determine weather a potential move is valid in the current or another given state of the game."""
    if state is None:
      state = self.currentstate;
    for a, b in state:
      if(step[0] >= a) &(step[1] >= b):
        return False;
    return True;
  def winning(self, state):
    """General poset game algorithm to determine if given state is winning by recursion with cache.
    A state is winning iff you can win if it is your turn to move from that state.
    A state is represented by a minimal list of cells that have to be moved to achieve it.
    Note that a list of lists is a very inefficient data structure for large boards:
    an n by m board has(n+m) choose n different possible states, and it is not easy to order them,
    therefore we can only search in the list by piecewise comparison.
    It would be a lot more efficient to store the path from the top left corner to the bottom right corner
    separating the removed and remaining pieces, possibly in binary as an integer,
    so we could sort the list of already calculated states, and use a much faster binary search for lookup."""
    # to make sure == matches identical states
    state.sort();
    # if we already calculated this state, return that result
    for a, b in self.statelist:
      if state == a:
        return b;
    if state == [[0,0]]:
      return True; # misère game: the person removing the last piece loses, they leave the oppenent a winning state
      #return False; # normal game: the person removing the last piece wins, they leave the oppenent a losing state
    # a state is winning iff we can leave our opponent a losing state
    answer = False;
    # cycle through all imaginable moves
    for i in range(self.n):
      for j in range(self.m):
        # i mean, all valid moves
        if self.valid([i,j], state):
          if not self.winning(self.move([i,j], state)):
            answer = True;
            break;
    # record if this state is winning
    self.statelist.append([state, answer]);
    return answer;
  def visualarray(self, hover=None):
    """Return an array of the board, identifying removed and remaining pieces,
    and the ones of the potential move if we clicked where the mouse is right now,
    so that they can be highlighted."""
    # if the mouse is not in the window, pretend it is to the right and above of the board
    if hover is None:
      hover = [self.n, self.m];
    answer = numpy.empty([self.n, self.m]);
    for i in range(self.n):
      for j in range(self.m):
        if self.valid([i,j]):
          if(i>=hover[0]) &(j>=hover[1]):
            answer [i,j] = 1;
          else:
            answer [i,j] = 0;
        else:
          answer [i,j] = 2;
    return answer;
  def lost(self):
    """Is the game over?"""
    return self.currentstate == [[0,0]];
  def playermoves(self, step):
    """The player makes a move, then the computer makes a move."""
    # if game over, this is a blind click, start new game
    if self.lost():
      self.startgame();
    # if game is still going, move if the step is valid
    elif self.valid(step):
      self.currentstate = self.move(step);
      # if player did not lose yet, let the computer move
      if not self.lost():
        self.computermoves();
    return;
  def computermoves(self):
    """Computer makes a move."""
    # if game over, noop
    if self.lost():
      return;
    # find all valid and winning moves
    validmoves = [];
    winningmoves = [];
    for i in range(self.n):
      for j in range(self.m):
        if self.valid([i, j]):
          if not self.winning(self.move([i, j])):
            winningmoves.append([i, j]);
    # if(n,m) !=(1,1), and the computer started, then we always have a winning move
    # pick one randomly
    if len(winningmoves) > 0:
      step = winningmoves [numpy.random.randint(len(winningmoves))];
    # otherwise, since the game is not over yet, we must have a valid move, so pick one randomly
    else:
      step = validmoves [numpy.random.randint(len(validmoves))];
    # and make this move
    self.currentstate = self.move(step);
    return;
  def move(self, step, state=None):
    """Calculate the next state after a move from the current or another state."""
    if state is None:
      state = self.currentstate;
    # if step is valid, append to newstate, and copy over all other elements not redundant with it
    if self.valid(step, state):
      newstate = [step];
      for i, j in state:
        if(i<step[0]) |(j<step[1]):
          newstate.append([i, j]);
      newstate.sort();
      return newstate;
    else:
      return state;

# init Tkinter
master = Tkinter.Tk();
# init a ChompGraphics object
ChompGraphicsInstance = ChompGraphics(master);
# bind functions from parent window
master.bind("<Configure>", bind_resize);
master.bind("<Leave>", bind_leave);
master.bind("<Motion>", bind_motion);
master.bind("<Button-1>", bind_click);
# start main loop
master.mainloop();
