#pragma once

#define __ALLOW_DEBUG_STAT_CHANGE		//��������޸ĵ���״̬

#define __ALLOW_DYMANIC_DEBUG			//����̬�޸ĵ���״̬

#ifndef __DEFAULT_DEBUG_STAT
#define __DEFAULT_DEBUG_STAT	true	//Ĭ�ϵ���״̬
#endif

//#define __WNO_OMP						//����OpenMPͷ

#define __ALLOW_LOWMEM_PREFER			//ʹ�õ��ڴ��Ż�����

#ifndef MTYPE
#define MTYPE	float					//��ѧ�⸡������
#endif

/*����*/
#define DEFAULT_DEBUG_STAT  __DEFAULT_DEBUG_STAT
