#ifndef OFFLINESVIEWIMPL_H
#define OFFLINESVIEWIMPL_H
#include "OfflinesView.h"

class MsgParser;

class OfflinesViewImpl : public OfflinesView
{ 
    Q_OBJECT

public:
    OfflinesViewImpl( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~OfflinesViewImpl();

public slots:
    void itemClicked(int,int,int,const QPoint&);

private:
    MsgParser *parse;
};

#endif // OFFLINESVIEWIMPL_H
