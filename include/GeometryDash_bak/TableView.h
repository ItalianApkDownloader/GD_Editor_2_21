#include "GeometryDash.h"
#include "cocos2d.h"
#include "cocos-ext.h"



class TableViewDelegate{
    virtual  bool willTweenToIndexPath(void *,TableViewCell *,TableView *);
    virtual  bool didEndTweenToIndexPath(void *,TableView *);
    virtual  void TableViewWillDisplayCellForRowAtIndexPath(void *,TableViewCell *,TableView *);
    virtual  void TableViewDidDisplayCellForRowAtIndexPath(void *,TableViewCell *,TableView *);
    virtual  void TableViewWillReloadCellForRowAtIndexPath(void *,TableViewCell *,TableView *);
};

class TableViewDataSource{
    virtual int numberOfSectionsInTableView(TableView *);
    //virtual void  TableViewCommitCellEditingStyleForRowAtIndexPath(TableView *,TableViewCellEditingStyle, &);
};


/* Main menu for user-created levels */
class TableView  {
public:
    void reloadData();
    void scrollWheel(float, float);

    static TableView * create(TableViewDelegate *,TableViewDataSource *,cocos2d::CCRect);
};