#include "inductor.h"
#include <QDebug>
Inductor::Inductor(unsigned int INDEX)
{
    index=INDEX;
    name="L"+QString::number(index);
    value="0";
    pixmap = new QPixmap(":/resources/inductor.png");
    width=pixmap->width();
    height=pixmap->height();
    bound=new QRectF(-width/2.,-height/2.-5,width,height+10);

    ports.append(QPoint(anker_x,anker_y-4));
    ports.append(QPoint(anker_x,anker_y+4));
}
Inductor::~Inductor(){
    index_list.remove(index);
    delete pixmap;
    delete bound;
}
QDataStream &operator<<(QDataStream &qs, const Inductor &i)
{
    qs << static_cast<const LinearComponent&>(i);
    qs << i.inductance << i.index_list;
    return qs;
}

QDataStream &operator>>(QDataStream &qs, Inductor &i)
{
    qs >> static_cast<LinearComponent&>(i);
    qs >> i.inductance >> i.index_list;
    return qs;
}
QRectF Inductor::boundingRect() const{
    return *bound;
}
void Inductor::paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->drawPixmap(-width/2,-height/2,*pixmap);
    qreal currentRotation = rotation();
   // qDebug() << "Rotation = " << currentRotation << endl;
    painter->rotate(-currentRotation);
    if(fabs(rotation()-0)<1e-6){
        painter->drawText(21,-46,name);
        painter->drawText(21,50,value);
    }else if(fabs(rotation()-90)<1e-6){
        painter->drawText(-55,-21,name);
        painter->drawText(42,-21,value);
    }else if(fabs(rotation()-180)<1e-6){
        painter->drawText(21,-46,name);
        painter->drawText(21,50,value);
    }else if(fabs(rotation()-270)<1e-6){
        painter->drawText(-55,-21,name);
        painter->drawText(42,-21,value);
    }
    painter->rotate(currentRotation);
}
void Inductor::rotate(){
    this->setRotation(this->rotation()+90);
    if(fabs(rotation()-360)<1e-6)
        this->setRotation(0);
    ports[0]=QPoint(-(ports[0].y()-anker_y)+anker_x,(ports[0].x()-anker_x)+anker_y); //rotate 90 degrees
    ports[1]=QPoint(-(ports[1].y()-anker_y)+anker_x,(ports[1].x()-anker_x)+anker_y);
//    qDebug() << port1 << " , " << port2 << endl;
}
void Inductor::moveTo(const QPointF scenePoint){
    QPoint oldAnker=QPoint(anker_x,anker_y);
    anker_x=round(scenePoint.x())/pixPerAnker;
    anker_y=round(scenePoint.y())/pixPerAnker;
    setPos(anker_x*pixPerAnker,anker_y*pixPerAnker);
    ports[0]+=getAnkerPoint()-oldAnker;
    ports[1]+=getAnkerPoint()-oldAnker;
}
void Inductor::set(QString cname,QString cvalue)
{
    name=cname;
    value=cvalue;
}

std::complex<double> Inductor::getimpedance(double  angularFrequency)
{
    using namespace std;
    complex<double> i(0.0,1.0);
    QString s=BasicComponent::alphabet(value);
    complex<double> imped=(s.toDouble()*angularFrequency)*i;
    return  imped;
}
QSet<unsigned> Inductor::index_list;

