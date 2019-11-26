global bool LEFT = true;
void straightAway() {
for (;;) {
    //go forward until an obstacle is reached
    while (look(FRONTLEFT)<THRESHOLD&&look(FRONTRIGHT)<THRESHOLD) {
        forward();
    }
    stop();
    if (makeTurnDecision() = ISLEFT) {
        scan(ISLEFT);
    }
    else 
        scan(ISRIGHT);
  }
}

bool makeTurnDecision() {
   return (look(LEFT)>look(RIGHT));
}

bool frontClear() {
    return (look(FRONTLEFT)<THRESHOLD&&look(FRONTRIGHT)<THRESHOLD);
}

void scan(bool leftright) {
    int trig;
    int echo;
    if(leftRight == ISLEFT) {
        trig = TLEFT;
        echo = ELEFT;
    }
    else {
        trig = TRIGHT;
        echo = ERIGHT;
    }
    
}
