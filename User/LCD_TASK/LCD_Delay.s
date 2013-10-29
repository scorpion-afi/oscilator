
    PUBLIC  Delay_asm
    SECTION LCD_DELAY:CODE(2)
    THUMB    

// !!!!!!!!!!!!!  r0 = n  !!!!!!!!!!!!!

// �������� �� ( 12 + n*(5) ) ������, ��� n = period   
// 1 ���� ~ 14 ����������
// �������� �� ( 12 + n*(5) )* 14 ����������, ��� n = period
// min ��������(n = 1) ~ 238 ���������� 
// n = (X - 168) / 70, ��� � - ��������� �������� � ������������
Delay_asm:
        // r0 = n (�� ���������� � �������� ���������� � �������)
        m_1:   
          cbz r0, m_exit            // ���� r0 == 0, �� ��������� �� m_exit
          subs r0, #1	            // r0--
          b m_1			    // ������ �� m_1
     m_exit:
          bx lr                        //������ �� �������, ����������� � lr
    END