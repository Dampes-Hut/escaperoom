DEFINE_MESSAGE(0x0001, TEXTBOX_TYPE_BLUE, TEXTBOX_POS_BOTTOM,
UNSKIPPABLE QUICKTEXT_ENABLE COLOR(RED) "MISSING MESSAGE DATA! (EN)" COLOR(DEFAULT) QUICKTEXT_DISABLE
,
UNSKIPPABLE QUICKTEXT_ENABLE COLOR(RED) "MISSING MESSAGE DATA! (DE)" COLOR(DEFAULT) QUICKTEXT_DISABLE
,
UNSKIPPABLE QUICKTEXT_ENABLE COLOR(RED) "MISSING MESSAGE DATA! (FR)" COLOR(DEFAULT) QUICKTEXT_DISABLE
)

#define DEFINE_MESSAGE_EN(textId, type, yPos, nesMessage)                                                      \
    DEFINE_MESSAGE(                                                                                            \
        textId, type, yPos, nesMessage,                                                                        \
        UNSKIPPABLE QUICKTEXT_ENABLE COLOR(RED) "MISSING MESSAGE DATA! (DE)" COLOR(DEFAULT) QUICKTEXT_DISABLE, \
        UNSKIPPABLE QUICKTEXT_ENABLE COLOR(RED) "MISSING MESSAGE DATA! (FR)" COLOR(DEFAULT) QUICKTEXT_DISABLE)

DEFINE_MESSAGE_EN(2, TEXTBOX_TYPE_BLACK, TEXTBOX_POS_MIDDLE,
UNSKIPPABLE QUICKTEXT_ENABLE COLOR(DEFAULT) "After a long day of adventuring..." QUICKTEXT_DISABLE
)

DEFINE_MESSAGE_EN(3, TEXTBOX_TYPE_BLACK, TEXTBOX_POS_MIDDLE,
UNSKIPPABLE QUICKTEXT_ENABLE COLOR(DEFAULT) "... Link is eager to get some rest." QUICKTEXT_DISABLE
)

DEFINE_MESSAGE_EN(4, TEXTBOX_TYPE_BLACK, TEXTBOX_POS_MIDDLE,
UNSKIPPABLE QUICKTEXT_ENABLE COLOR(DEFAULT) "As the night is falling..." QUICKTEXT_DISABLE
)

DEFINE_MESSAGE_EN(5, TEXTBOX_TYPE_BLACK, TEXTBOX_POS_MIDDLE,
UNSKIPPABLE QUICKTEXT_ENABLE COLOR(DEFAULT) "... Link encounters an inn." QUICKTEXT_DISABLE
)

DEFINE_MESSAGE_EN(6, TEXTBOX_TYPE_BLACK, TEXTBOX_POS_MIDDLE,
UNSKIPPABLE QUICKTEXT_ENABLE COLOR(DEFAULT) "He pays for a room until morning..." QUICKTEXT_DISABLE
)

DEFINE_MESSAGE_EN(7, TEXTBOX_TYPE_BLACK, TEXTBOX_POS_MIDDLE,
UNSKIPPABLE QUICKTEXT_ENABLE COLOR(DEFAULT) "... and heads to his night quarters." QUICKTEXT_DISABLE
)


DEFINE_MESSAGE_EN(100, TEXTBOX_TYPE_BLACK, TEXTBOX_POS_MIDDLE,
UNSKIPPABLE QUICKTEXT_ENABLE COLOR(DEFAULT) "You found a key!\nIt can open a locked door." QUICKTEXT_DISABLE
)

/*
z<-o  oot world axes
   |
 x v

courtyard graves layout

 0
1
2     5 7
3         6
     8
4         9
*/

// 8 and 9: push 9
DEFINE_MESSAGE_EN(200, TEXTBOX_TYPE_BLACK, TEXTBOX_POS_MIDDLE,
UNSKIPPABLE QUICKTEXT_ENABLE COLOR(DEFAULT) "Ten souls lie buried.\nOf the two facing each other, do not disturb the one near the wall." QUICKTEXT_DISABLE
)

// pull 0
DEFINE_MESSAGE_EN(201, TEXTBOX_TYPE_BLACK, TEXTBOX_POS_MIDDLE,
UNSKIPPABLE QUICKTEXT_ENABLE COLOR(DEFAULT) "Ten souls lie buried.\nThe one nearest to the coffins should be troubled." QUICKTEXT_DISABLE
)

// forbidden
// pull 4
DEFINE_MESSAGE_EN(202, TEXTBOX_TYPE_BLACK, TEXTBOX_POS_MIDDLE,
UNSKIPPABLE QUICKTEXT_ENABLE COLOR(DEFAULT) "Ten souls lie buried.\nBother the one closest to the library." QUICKTEXT_DISABLE
)

// 0, 1, 2, 3: push at least two
DEFINE_MESSAGE_EN(203, TEXTBOX_TYPE_BLACK, TEXTBOX_POS_MIDDLE,
UNSKIPPABLE QUICKTEXT_ENABLE COLOR(DEFAULT) "Ten souls lie buried, four side by side.\nAt least two of them should be left in peace." QUICKTEXT_DISABLE
)

// forbidden
// 0, 1, 2, 3: any one may not be surrounded by two pushed ones (one on each side)
DEFINE_MESSAGE_EN(204, TEXTBOX_TYPE_BLACK, TEXTBOX_POS_MIDDLE,
UNSKIPPABLE QUICKTEXT_ENABLE COLOR(DEFAULT) "Ten souls lie buried, four side by side.\nNo one may be surrounded on both sides by harmony." QUICKTEXT_DISABLE
)

// 0, 1, 2, 3: there may not be 2 pulled ones side by side
DEFINE_MESSAGE_EN(205, TEXTBOX_TYPE_BLACK, TEXTBOX_POS_MIDDLE,
UNSKIPPABLE QUICKTEXT_ENABLE COLOR(DEFAULT) "Ten souls lie buried, four side by side.\nA troubled one may not be a neighbour to another one of its kind." QUICKTEXT_DISABLE
)

// pull 8 (near the tree)
DEFINE_MESSAGE_EN(206, TEXTBOX_TYPE_BLACK, TEXTBOX_POS_MIDDLE,
UNSKIPPABLE QUICKTEXT_ENABLE COLOR(DEFAULT) "Ten souls lie buried.\nDisturb the one near what is dead yet not buried." QUICKTEXT_DISABLE
)

// push 5 (has mc poppies)
DEFINE_MESSAGE_EN(207, TEXTBOX_TYPE_BLACK, TEXTBOX_POS_MIDDLE,
UNSKIPPABLE QUICKTEXT_ENABLE COLOR(DEFAULT) "Ten souls lie buried.\nShow respect where flowers bloom." QUICKTEXT_DISABLE
)

// forbidden
// pull 6 (slower to push/pull)
DEFINE_MESSAGE_EN(208, TEXTBOX_TYPE_BLACK, TEXTBOX_POS_MIDDLE,
UNSKIPPABLE QUICKTEXT_ENABLE COLOR(DEFAULT) "Ten souls lie buried.\nTrouble the one carrying heavy misdeeds." QUICKTEXT_DISABLE
)

// forbidden
// 5 and 7: set 7 like 5 (push)
DEFINE_MESSAGE_EN(209, TEXTBOX_TYPE_BLACK, TEXTBOX_POS_MIDDLE,
UNSKIPPABLE QUICKTEXT_ENABLE COLOR(DEFAULT) "Ten souls lie buried.\nThe two closest to the tower should not be separated." QUICKTEXT_DISABLE
)



#ifdef DEFINE_MESSAGE_FFFC
DEFINE_MESSAGE(0xFFFC, TEXTBOX_TYPE_BLACK, TEXTBOX_POS_VARIABLE,
"0123456789\n"
"ABCDEFGHIJKLMN\n"
"OPQRSTUVWXYZ\n"
"abcdefghijklmn\n"
"opqrstuvwxyz\n"
" -.\n"
,
,
)
#endif

DEFINE_MESSAGE(0xFFFD, TEXTBOX_TYPE_BLACK, TEXTBOX_POS_VARIABLE,
""
,
""
,
""
)
