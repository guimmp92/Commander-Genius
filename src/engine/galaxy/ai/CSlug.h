/*
 * CPlatform.h
 *
 *  Created on: 23.02.2011
 *      Author: FCTW
 *
 *  This handles the Poison Slugs in Galaxy
 */
 
#ifndef CSLUG_H_
#define CSLUG_H_

#include "common/CObject.h"

namespace galaxy {

#define A_SLUG_MOVE	0	/* Ordinary slug */

class CSlug : public CObject {
public:
	CSlug(CMap *pmap, Uint32 x, Uint32 y);
	void process();
};

}

#endif /* CSLUG_H_ */
