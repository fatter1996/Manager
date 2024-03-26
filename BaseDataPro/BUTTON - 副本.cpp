#include "BUTTON.h"

CBUTTON::CBUTTON(void)
{
    m_nState = 0;
}

CBUTTON::~CBUTTON(void)
{
}

void CBUTTON::BUTTONInit()
{
    if (m_nType == BLOCK_BUTTON)
    {
        m_strName = "����";
    }
    else if (m_nType == RECOVER_BUTTON)
    {
        m_strName = "��ԭ";
    }
    else
    {
        m_strName = "�¹�";
    }
    m_rect.setRect(pCenter.x(), pCenter.y(), 15, 15);
}

void CBUTTON::setState(BYTE nState)
{
    m_nState = nState;
}

bool CBUTTON::getState(int nState)
{
    if ((int)(m_nState & nState) > 0)  //�ж����״̬�Ƿ��������״̬
        return true;
    else
        return false;
}
void CBUTTON::DRAW(QPainter *painter, float nDiploid, int nElapsed)
{
    QFont font;
    QString image = "";
    if (m_nType == BLOCK_BUTTON)
    {
        if (getState(ALL)) //���Ʊ����µİ�ť
        {
            image = ":/Pictures/LED_Red.jpg"; //up
        }
        else //����̧��İ�ť
        {
            image = ":/Pictures/LED_Red.jpg";
        }
    }
    else if (m_nType == RECOVER_BUTTON)
    {
        if (getState(ALL)) //���Ʊ����µİ�ť
        {
            image = ":/Pictures/LED_Red.jpg"; //up
        }
        else //����̧��İ�ť
        {
            image = ":/Pictures/LED_Red.jpg";
        }
    }
    else if (m_nType == FAULT_BUTTON)
    {
        if (getState(ALL)) //���Ʊ����µİ�ť
        {
            image = ":/Pictures/LED_Red.jpg"; //up
        }
        else //����̧��İ�ť
        {
            image = ":/Pictures/LED_Red.jpg";
        }
    }
    draw_Pixmap(painter, ":/Pictures/LED_Red.jpg", m_rect.topLeft().x(), m_rect.topLeft().x(), 15, 15, nDiploid);

    font.setFamily("����");
    font.setPointSize(10 * nDiploid);//�ֺ�
    font.setItalic(false);//б��
    painter->setFont(font);//��������
    painter->setPen(Qt::white);//���û�����ɫ
    painter->drawText(m_textRect, m_strName);
}

int CBUTTON::moveCursor(QPoint p)
{
    if (m_rect.contains(p))
        return 2;
    if (m_textRect.contains(p))
        return 3;
    return 0;
}

