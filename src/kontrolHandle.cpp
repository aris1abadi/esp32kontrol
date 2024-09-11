#include <Arduino.h>
#include <timeHandle.h>
#include <nodeHandle.h>
#include <mySensorsHandle.h>

extern Aktuator aktuator1;
extern Aktuator aktuator2;
extern Aktuator aktuator3;
extern Aktuator aktuator4;
extern Aktuator aktuator5;
extern Aktuator aktuator6;
extern Aktuator aktuator7;
extern Aktuator aktuator8;

extern Sensor sensor1;
extern Sensor sensor2;
extern Sensor sensor3;
extern Sensor sensor4;
extern Sensor sensor5;
extern Sensor sensor6;
extern Sensor sensor7;
extern Sensor sensor8;

void cek_jadwalNode()
{
    if (isTimeUpdate)
    {
        setTimeUpdate(false);

        // cek jadwal node
        if (aktuator1.status == 1)
        {
            if (aktuator1.val == 0)
            {
                bool isOn = false;
                // cek jadwal
                // durasi 0 timer disable
                if ((aktuator1.jadwal[2] != 0) && (aktuator1.jadwal[0] == getJam()) && (aktuator1.jadwal[1] == getMenit()))
                {
                    isOn = true;
                    aktuator1.jadwalAktif = 2;
                }
                if ((aktuator1.jadwal[5] != 0) && (aktuator1.jadwal[3] == getJam()) && (aktuator1.jadwal[4] == getMenit()))
                {
                    isOn = true;
                    aktuator1.jadwalAktif = 5;
                }
                if ((aktuator1.jadwal[8] != 0) && (aktuator1.jadwal[6] == getJam()) && (aktuator1.jadwal[7] == getMenit()))
                {
                    isOn = true;
                    aktuator1.jadwalAktif = 8;
                }
                if ((aktuator1.jadwal[11] != 0) && (aktuator1.jadwal[9] == getJam()) && (aktuator1.jadwal[10] == getMenit()))
                {
                    isOn = true;
                    aktuator1.jadwalAktif = 11;
                }
                if ((aktuator1.jadwal[14] != 0) && (aktuator1.jadwal[12] == getJam()) && (aktuator1.jadwal[13] == getMenit()))
                {
                    isOn = true;
                    aktuator1.jadwalAktif = 14;
                }

                if (isOn)
                {
                    // kirim On ke aktuator
                    kontrolAktuator(1, 1);
                    aktuator1.durasiCount = 0;
                    Serial.println("Aktuator1 ON(timer)");
                }
            }
            else
            {
                // countDown Durasi
                if (aktuator1.jadwalAktif == 1)
                {
                    // manual  durasi ON
                    if (++aktuator1.durasiCount > aktuator1.durasiManual)
                    {
                        aktuator1.durasiCount = 0;
                        aktuator1.jadwalAktif = 0;
                        kontrolAktuator(1, 0);
                        Serial.println("Aktuator1 OFF(Manual)");
                    }
                }
                else
                {
                    if (aktuator1.jadwalAktif != 0)
                    {
                        if (++aktuator1.durasiCount > aktuator1.jadwal[aktuator1.jadwalAktif])
                        {
                            aktuator1.durasiCount = 0;
                            aktuator1.jadwalAktif = 0;
                            kontrolAktuator(1, 0);
                            Serial.println("Aktuator1 OFF(Timer)");
                        }
                    }
                }
            }
        }
        else if (aktuator2.status == 1)
        {
            if (aktuator2.val == 0)
            {
                bool isOn = false;
                // cek jadwal
                // durasi 0 timer disable
                if ((aktuator2.jadwal[2] != 0) && (aktuator2.jadwal[0] == getJam()) && (aktuator2.jadwal[1] == getMenit()))
                {
                    isOn = true;
                    aktuator2.jadwalAktif = 2;
                }
                if ((aktuator2.jadwal[5] != 0) && (aktuator2.jadwal[3] == getJam()) && (aktuator2.jadwal[4] == getMenit()))
                {
                    isOn = true;
                    aktuator2.jadwalAktif = 5;
                }
                if ((aktuator2.jadwal[8] != 0) && (aktuator2.jadwal[6] == getJam()) && (aktuator2.jadwal[7] == getMenit()))
                {
                    isOn = true;
                    aktuator2.jadwalAktif = 8;
                }
                if ((aktuator2.jadwal[11] != 0) && (aktuator2.jadwal[9] == getJam()) && (aktuator2.jadwal[10] == getMenit()))
                {
                    isOn = true;
                    aktuator2.jadwalAktif = 11;
                }
                if ((aktuator2.jadwal[14] != 0) && (aktuator2.jadwal[12] == getJam()) && (aktuator2.jadwal[13] == getMenit()))
                {
                    isOn = true;
                    aktuator2.jadwalAktif = 14;
                }

                if (isOn)
                {
                    // kirim On ke aktuator
                    kontrolAktuator(2, 1);
                    aktuator2.durasiCount = 0;
                    Serial.println("aktuator2 ON(timer)");
                }
            }
            else
            {
                // countDown Durasi
                if (aktuator2.jadwalAktif == 1)
                {
                    // manual  durasi ON
                    if (++aktuator2.durasiCount > aktuator2.durasiManual)
                    {
                        aktuator2.durasiCount = 0;
                        aktuator2.jadwalAktif = 0;
                        kontrolAktuator(2, 0);
                        Serial.println("aktuator2 OFF(Manual)");
                    }
                }
                else
                {
                    if (aktuator2.jadwalAktif != 0)
                    {
                        if (++aktuator2.durasiCount > aktuator2.jadwal[aktuator2.jadwalAktif])
                        {
                            aktuator2.durasiCount = 0;
                            aktuator2.jadwalAktif = 0;
                            kontrolAktuator(2, 0);
                            Serial.println("aktuator2 OFF(Timer)");
                        }
                    }
                }
            }
        }
        else if (aktuator3.status == 1)
        {
            if (aktuator3.val == 0)
            {
                bool isOn = false;
                // cek jadwal
                // durasi 0 timer disable
                if ((aktuator3.jadwal[2] != 0) && (aktuator3.jadwal[0] == getJam()) && (aktuator3.jadwal[1] == getMenit()))
                {
                    isOn = true;
                    aktuator3.jadwalAktif = 2;
                }
                if ((aktuator3.jadwal[5] != 0) && (aktuator3.jadwal[3] == getJam()) && (aktuator3.jadwal[4] == getMenit()))
                {
                    isOn = true;
                    aktuator3.jadwalAktif = 5;
                }
                if ((aktuator3.jadwal[8] != 0) && (aktuator3.jadwal[6] == getJam()) && (aktuator3.jadwal[7] == getMenit()))
                {
                    isOn = true;
                    aktuator3.jadwalAktif = 8;
                }
                if ((aktuator3.jadwal[11] != 0) && (aktuator3.jadwal[9] == getJam()) && (aktuator3.jadwal[10] == getMenit()))
                {
                    isOn = true;
                    aktuator3.jadwalAktif = 11;
                }
                if ((aktuator3.jadwal[14] != 0) && (aktuator3.jadwal[12] == getJam()) && (aktuator3.jadwal[13] == getMenit()))
                {
                    isOn = true;
                    aktuator3.jadwalAktif = 14;
                }

                if (isOn)
                {
                    // kirim On ke aktuator
                    kontrolAktuator(3, 1);
                    aktuator3.durasiCount = 0;
                    Serial.println("aktuator3 ON(timer)");
                }
            }
            else
            {
                // countDown Durasi
                if (aktuator3.jadwalAktif == 1)
                {
                    // manual  durasi ON
                    if (++aktuator3.durasiCount > aktuator3.durasiManual)
                    {
                        aktuator3.durasiCount = 0;
                        aktuator3.jadwalAktif = 0;
                        kontrolAktuator(3, 0);
                        Serial.println("aktuator3 OFF(Manual)");
                    }
                }
                else
                {
                    if (aktuator3.jadwalAktif != 0)
                    {
                        if (++aktuator3.durasiCount > aktuator3.jadwal[aktuator3.jadwalAktif])
                        {
                            aktuator3.durasiCount = 0;
                            aktuator3.jadwalAktif = 0;
                            kontrolAktuator(3, 0);
                            Serial.println("aktuator3 OFF(Timer)");
                        }
                    }
                }
            }
        }
        else if (aktuator4.status == 1)
        {
            if (aktuator4.val == 0)
            {
                bool isOn = false;
                // cek jadwal
                // durasi 0 timer disable
                if ((aktuator4.jadwal[2] != 0) && (aktuator4.jadwal[0] == getJam()) && (aktuator4.jadwal[1] == getMenit()))
                {
                    isOn = true;
                    aktuator4.jadwalAktif = 2;
                }
                if ((aktuator4.jadwal[5] != 0) && (aktuator4.jadwal[3] == getJam()) && (aktuator4.jadwal[4] == getMenit()))
                {
                    isOn = true;
                    aktuator4.jadwalAktif = 5;
                }
                if ((aktuator4.jadwal[8] != 0) && (aktuator4.jadwal[6] == getJam()) && (aktuator4.jadwal[7] == getMenit()))
                {
                    isOn = true;
                    aktuator4.jadwalAktif = 8;
                }
                if ((aktuator4.jadwal[11] != 0) && (aktuator4.jadwal[9] == getJam()) && (aktuator4.jadwal[10] == getMenit()))
                {
                    isOn = true;
                    aktuator4.jadwalAktif = 11;
                }
                if ((aktuator4.jadwal[14] != 0) && (aktuator4.jadwal[12] == getJam()) && (aktuator4.jadwal[13] == getMenit()))
                {
                    isOn = true;
                    aktuator4.jadwalAktif = 14;
                }

                if (isOn)
                {
                    // kirim On ke aktuator
                    kontrolAktuator(4, 1);
                    aktuator4.durasiCount = 0;
                    Serial.println("aktuator4 ON(timer)");
                }
            }
            else
            {
                // countDown Durasi
                if (aktuator4.jadwalAktif == 1)
                {
                    // manual  durasi ON
                    if (++aktuator4.durasiCount > aktuator4.durasiManual)
                    {
                        aktuator4.durasiCount = 0;
                        aktuator4.jadwalAktif = 0;
                        kontrolAktuator(4, 0);
                        Serial.println("aktuator4 OFF(Manual)");
                    }
                }
                else
                {
                    if (aktuator4.jadwalAktif != 0)
                    {
                        if (++aktuator4.durasiCount > aktuator4.jadwal[aktuator4.jadwalAktif])
                        {
                            aktuator4.durasiCount = 0;
                            aktuator4.jadwalAktif = 0;
                            kontrolAktuator(4, 0);
                            Serial.println("aktuator4 OFF(Timer)");
                        }
                    }
                }
            }
        }
        else if (aktuator5.status == 1)
        {
            if (aktuator5.val == 0)
            {
                bool isOn = false;
                // cek jadwal
                // durasi 0 timer disable
                if ((aktuator5.jadwal[2] != 0) && (aktuator5.jadwal[0] == getJam()) && (aktuator5.jadwal[1] == getMenit()))
                {
                    isOn = true;
                    aktuator5.jadwalAktif = 2;
                }
                if ((aktuator5.jadwal[5] != 0) && (aktuator5.jadwal[3] == getJam()) && (aktuator5.jadwal[4] == getMenit()))
                {
                    isOn = true;
                    aktuator5.jadwalAktif = 5;
                }
                if ((aktuator5.jadwal[8] != 0) && (aktuator5.jadwal[6] == getJam()) && (aktuator5.jadwal[7] == getMenit()))
                {
                    isOn = true;
                    aktuator5.jadwalAktif = 8;
                }
                if ((aktuator5.jadwal[11] != 0) && (aktuator5.jadwal[9] == getJam()) && (aktuator5.jadwal[10] == getMenit()))
                {
                    isOn = true;
                    aktuator5.jadwalAktif = 11;
                }
                if ((aktuator5.jadwal[14] != 0) && (aktuator5.jadwal[12] == getJam()) && (aktuator5.jadwal[13] == getMenit()))
                {
                    isOn = true;
                    aktuator5.jadwalAktif = 14;
                }

                if (isOn)
                {
                    // kirim On ke aktuator
                    kontrolAktuator(5, 1);
                    aktuator5.durasiCount = 0;
                    Serial.println("aktuator5 ON(timer)");
                }
            }
            else
            {
                // countDown Durasi
                if (aktuator5.jadwalAktif == 1)
                {
                    // manual  durasi ON
                    if (++aktuator5.durasiCount > aktuator5.durasiManual)
                    {
                        aktuator5.durasiCount = 0;
                        aktuator5.jadwalAktif = 0;
                        kontrolAktuator(5, 0);
                        Serial.println("aktuator5 OFF(Manual)");
                    }
                }
                else
                {
                    if (aktuator5.jadwalAktif != 0)
                    {
                        if (++aktuator5.durasiCount > aktuator5.jadwal[aktuator5.jadwalAktif])
                        {
                            aktuator5.durasiCount = 0;
                            aktuator5.jadwalAktif = 0;
                            kontrolAktuator(5, 0);
                            Serial.println("aktuator5 OFF(Timer)");
                        }
                    }
                }
            }
        }
        else if (aktuator6.status == 1)
        {
            if (aktuator6.val == 0)
            {
                bool isOn = false;
                // cek jadwal
                // durasi 0 timer disable
                if ((aktuator6.jadwal[2] != 0) && (aktuator6.jadwal[0] == getJam()) && (aktuator6.jadwal[1] == getMenit()))
                {
                    isOn = true;
                    aktuator6.jadwalAktif = 2;
                }
                if ((aktuator6.jadwal[5] != 0) && (aktuator6.jadwal[3] == getJam()) && (aktuator6.jadwal[4] == getMenit()))
                {
                    isOn = true;
                    aktuator6.jadwalAktif = 5;
                }
                if ((aktuator6.jadwal[8] != 0) && (aktuator6.jadwal[6] == getJam()) && (aktuator6.jadwal[7] == getMenit()))
                {
                    isOn = true;
                    aktuator6.jadwalAktif = 8;
                }
                if ((aktuator6.jadwal[11] != 0) && (aktuator6.jadwal[9] == getJam()) && (aktuator6.jadwal[10] == getMenit()))
                {
                    isOn = true;
                    aktuator6.jadwalAktif = 11;
                }
                if ((aktuator6.jadwal[14] != 0) && (aktuator6.jadwal[12] == getJam()) && (aktuator6.jadwal[13] == getMenit()))
                {
                    isOn = true;
                    aktuator6.jadwalAktif = 14;
                }

                if (isOn)
                {
                    // kirim On ke aktuator
                    kontrolAktuator(6, 1);
                    aktuator6.durasiCount = 0;
                    Serial.println("aktuator6 ON(timer)");
                }
            }
            else
            {
                // countDown Durasi
                if (aktuator6.jadwalAktif == 1)
                {
                    // manual  durasi ON
                    if (++aktuator6.durasiCount > aktuator6.durasiManual)
                    {
                        aktuator6.durasiCount = 0;
                        aktuator6.jadwalAktif = 0;
                        kontrolAktuator(6, 0);
                        Serial.println("aktuator6 OFF(Manual)");
                    }
                }
                else
                {
                    if (aktuator6.jadwalAktif != 0)
                    {
                        if (++aktuator6.durasiCount > aktuator6.jadwal[aktuator6.jadwalAktif])
                        {
                            aktuator6.durasiCount = 0;
                            aktuator6.jadwalAktif = 0;
                            kontrolAktuator(6, 0);
                            Serial.println("aktuator6 OFF(Timer)");
                        }
                    }
                }
            }
        }
        else if (aktuator7.status == 1)
        {
            if (aktuator7.val == 0)
            {
                bool isOn = false;
                // cek jadwal
                // durasi 0 timer disable
                if ((aktuator7.jadwal[2] != 0) && (aktuator7.jadwal[0] == getJam()) && (aktuator7.jadwal[1] == getMenit()))
                {
                    isOn = true;
                    aktuator7.jadwalAktif = 2;
                }
                if ((aktuator7.jadwal[5] != 0) && (aktuator7.jadwal[3] == getJam()) && (aktuator7.jadwal[4] == getMenit()))
                {
                    isOn = true;
                    aktuator7.jadwalAktif = 5;
                }
                if ((aktuator7.jadwal[8] != 0) && (aktuator7.jadwal[6] == getJam()) && (aktuator7.jadwal[7] == getMenit()))
                {
                    isOn = true;
                    aktuator7.jadwalAktif = 8;
                }
                if ((aktuator7.jadwal[11] != 0) && (aktuator7.jadwal[9] == getJam()) && (aktuator7.jadwal[10] == getMenit()))
                {
                    isOn = true;
                    aktuator7.jadwalAktif = 11;
                }
                if ((aktuator7.jadwal[14] != 0) && (aktuator7.jadwal[12] == getJam()) && (aktuator7.jadwal[13] == getMenit()))
                {
                    isOn = true;
                    aktuator7.jadwalAktif = 14;
                }

                if (isOn)
                {
                    // kirim On ke aktuator
                    kontrolAktuator(7, 1);
                    aktuator7.durasiCount = 0;
                    Serial.println("aktuator7 ON(timer)");
                }
            }
            else
            {
                // countDown Durasi
                if (aktuator7.jadwalAktif == 1)
                {
                    // manual  durasi ON
                    if (++aktuator7.durasiCount > aktuator7.durasiManual)
                    {
                        aktuator7.durasiCount = 0;
                        aktuator7.jadwalAktif = 0;
                        kontrolAktuator(7, 0);
                        Serial.println("aktuator7 OFF(Manual)");
                    }
                }
                else
                {
                    if (aktuator7.jadwalAktif != 0)
                    {
                        if (++aktuator7.durasiCount > aktuator7.jadwal[aktuator7.jadwalAktif])
                        {
                            aktuator7.durasiCount = 0;
                            aktuator7.jadwalAktif = 0;
                            kontrolAktuator(7, 0);
                            Serial.println("aktuator7 OFF(Timer)");
                        }
                    }
                }
            }
        }
        else if (aktuator8.status == 1)
        {
            if (aktuator8.val == 0)
            {
                bool isOn = false;
                // cek jadwal
                // durasi 0 timer disable
                if ((aktuator8.jadwal[2] != 0) && (aktuator8.jadwal[0] == getJam()) && (aktuator8.jadwal[1] == getMenit()))
                {
                    isOn = true;
                    aktuator8.jadwalAktif = 2;
                }
                if ((aktuator8.jadwal[5] != 0) && (aktuator8.jadwal[3] == getJam()) && (aktuator8.jadwal[4] == getMenit()))
                {
                    isOn = true;
                    aktuator8.jadwalAktif = 5;
                }
                if ((aktuator8.jadwal[8] != 0) && (aktuator8.jadwal[6] == getJam()) && (aktuator8.jadwal[7] == getMenit()))
                {
                    isOn = true;
                    aktuator8.jadwalAktif = 8;
                }
                if ((aktuator8.jadwal[11] != 0) && (aktuator8.jadwal[9] == getJam()) && (aktuator8.jadwal[10] == getMenit()))
                {
                    isOn = true;
                    aktuator8.jadwalAktif = 11;
                }
                if ((aktuator8.jadwal[14] != 0) && (aktuator8.jadwal[12] == getJam()) && (aktuator8.jadwal[13] == getMenit()))
                {
                    isOn = true;
                    aktuator8.jadwalAktif = 14;
                }

                if (isOn)
                {
                    // kirim On ke aktuator
                    kontrolAktuator(8, 1);
                    aktuator8.durasiCount = 0;
                    Serial.println("aktuator8 ON(timer)");
                }
            }
            else
            {
                // countDown Durasi
                if (aktuator8.jadwalAktif == 1)
                {
                    // manual  durasi ON
                    if (++aktuator8.durasiCount > aktuator8.durasiManual)
                    {
                        aktuator8.durasiCount = 0;
                        aktuator8.jadwalAktif = 0;
                        kontrolAktuator(8, 0);
                        Serial.println("aktuator8 OFF(Manual)");
                    }
                }
                else
                {
                    if (aktuator8.jadwalAktif != 0)
                    {
                        if (++aktuator8.durasiCount > aktuator8.jadwal[aktuator8.jadwalAktif])
                        {
                            aktuator8.durasiCount = 0;
                            aktuator8.jadwalAktif = 0;
                            kontrolAktuator(8, 0);
                            Serial.println("aktuator8 OFF(Timer)");
                        }
                    }
                }
            }
        }
        
    }
}

void kontrol_loop()
{
    cek_jadwalNode();
}
