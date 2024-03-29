#include "bullet.h"
#include <math.h>
#include<zombie.h>
#include<widget.h>
static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

static qreal normalizeAngle(qreal angle)
{
    while (angle < 0)
        angle += TwoPi;
    while (angle > TwoPi)
        angle -= TwoPi;
    return angle;
}

Bullet::Bullet(QPointF start, QPointF end, QObject *parent)
    : QObject(parent), QGraphicsItem()
{
    this->setRotation(0);   // Устанавливаем начальный разворот
    this->setPos(start);    // Устанавливаем стартовую позицию
    // Определяем траекторию полёта пули
    QLineF lineToTarget(start, end);
    // Угол поворота в направлении к цели
    qreal angleToTarget = ::acos(lineToTarget.dx() / lineToTarget.length());
    if (lineToTarget.dy() < 0)
        angleToTarget = TwoPi - angleToTarget;
    angleToTarget = normalizeAngle((Pi - angleToTarget) + Pi / 2);

    /* Разворачиваем пули по траектории
     * */
    if (angleToTarget >= 0 && angleToTarget < Pi) {
        /// Rotate left
        setRotation(rotation() - angleToTarget * 180 /Pi);
    } else if (angleToTarget <= TwoPi && angleToTarget > Pi) {
        /// Rotate right
        setRotation(rotation() + (angleToTarget - TwoPi )* (-180) /Pi);
    }
    setPos(mapToParent(10, -40));
    // Инициализируем полётный таймер
    timerBullet = new QTimer();
    // И подключаем его к слоту для обработки полёта пули
    connect(timerBullet, &QTimer::timeout, this, &Bullet::slotTimerBullet);
    timerBullet->start(15);
}

Bullet::~Bullet()
{

}

//Bullet::~Bullet()
//{

//}

QRectF Bullet::boundingRect() const
{
    return QRectF(-10,-10,20,20);//НУЖНО ИСПРАВИТЬ
}

void Bullet::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::black);
    painter->setBrush(Qt::yellow);
    painter->drawEllipse(-4,-4,8,8);

    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void Bullet::slotTimerBullet()
{
    setPos(mapToParent(0, -50));

    /* Проверка выхода за границы поля
     * Если пуля вылетает за заданные границы, то пулю необходимо уничтожить
     * */
    if(this->x() < -3000){
        this->deleteLater();
    }
    if(this->x() > 3000){
        this->deleteLater();
    }

    if(this->y() < -3000){
        this->deleteLater();
    }
    if(this->y() > 3000){
        this->deleteLater();
    }


    QList<QGraphicsItem *> colliding = scene()->collidingItems(this);
    for (QGraphicsItem* item: colliding) {
        if (dynamic_cast<Zombie*> (item)){
            this->deleteLater();
            emit signalBullet(item);
        }
    }

}
