#include "CollisionFix.h"
#include <bits/stdc++.h>
#include "hooking.h"
#include "cocos2d.h"

using namespace cocos2d;

#define FUNCTIONHOOK(returntype, name, ...) \
returntype (*name##O)(__VA_ARGS__);			\
returntype name##H(__VA_ARGS__)

// - object collision
/*
	oh my fucking god im never doing collisions again 
	- Catto
*/
// TODO: fix pads and slopes and shit and other shit idfk
FUNCTIONHOOK(void, PlayerObject_collidedWithObjectInternal, PlayerObject* self, float idk, GameObject* obj, CCRect hitbox, bool what) {
	if(self->_isGameplayRotated() && !GM->getGameVariable("1000010")) {
		int idkSomeInt = obj != nullptr ? MBO(int, obj, 0x324) : 0;
 
		CCRect rect = hitbox;
		GameObject* object = nullptr;

		if (obj != nullptr && hitbox.equals(CCRect(0, 0, 0, 0))) {
			rect = obj->getObjectRect();
			object = obj;
		}

		auto playerRect = self->getObjectRect(0.3, 0.3);
		auto playerRect2 = self->getObjectRect();
		bool isPassableObj = false;
		auto pos = self->getPosition();

		// some calcs idfk
		float mul1 = 10;
		if(self->_isPlatformer() || MBO(GameObject*, self, 0x749) == nullptr) {
			if(!MBO(bool, self, 0x748)) {
				mul1 = 5;
			}
		}
		float mul2 = mul1 * self->flipMod();
		if(self->isFlying() && !self->_isPlatformer()) mul2 = self->flipMod() * 6;
		
		if(MBO(GameObject*, self, 0x749) != nullptr) {
			mul2 = mul2 + MBO(double, self, 0x578) * self->flipMod();
		}
		if(object != nullptr) {
			isPassableObj = MBO(bool, object, 0x4AE);
		}

		if(isPassableObj) return; // we dont want any collision action with passable objects

		// i have no fucking idea what this does or what the values even are
		/*if(obj != nullptr) {
			if(idk > 0) {
				auto lastPos = obj->getLastPosition();
				auto realPos = obj->getRealPosition();
				auto scaleD = obj->getScalePosDelta();

				float thang = scaleD.x;
				float thang2 = realPos.x;
				if(scaleD.y != 0) {
					auto pPos = self->getPosition();

					float v19 = 0;

					if(pPos.x >= scaleD.x) v19 = thang2 - thang;
					else v19 = thang2 + thang;

					thang = 0;
					thang2 = v19;
				}
				if(realPos != lastPos || scaleD.x != thang2 || thang != 0) {

				}
			}
		}*/

		float v31 = MBO(float, self, 0x7C8) * 0.5;
		float v32 = pos.x - v31 * self->flipMod();
		float v34 = v32 + mul2;
		float v35 = pos.x - v31 * self->flipMod() + mul2;

		// H blocks and platformer
		bool hitHead = false;
		if(MBO(int, self, 0xB7C) > 0 || self->_isPlatformer() || MBO(int, self, 0xB80) > 0) {
			hitHead = !isPassableObj;
		}

		auto point2 = MBO(CCPoint, self, 0x468);

		bool isFlipGravity = self->_isGravityFlipped();
		if (isFlipGravity)
			isFlipGravity = self->isFlying();

		// some shit
		bool isRollMode = true;
		if(self->_isGravityFlipped()) {
			if(self->isFlying() || self->_isRollMode())
				isRollMode = true;
			else isRollMode = !self->_isRollMode();
		}

		bool noJump = true;
		bool isDblock = true;
		if(self->_isDartMode() && MBO(int, self, 0xB78) <= 0) {
			isDblock = false;
			noJump = false;
		}

		// test
		/*if(self->_yVelocity() <= 0 || MBO(bool, self, 0x749)) {
			//goto DOSTUFF;
		}*/

		//CCLog("v34: %f, v35: %f, objectRect: (MinX: %f, MaxX: %f, MinY: %f, MaxY: %f)", v34, v35, rect.getMinX(), rect.getMaxX(), rect.getMinY(), rect.getMaxY());

		if(self->_isGravityFlipped()) {
			if(v34 > rect.getMinX() && v35 > rect.getMinX()) {
				noJump = false;
				goto DOSTUFF;
			}
		}
		else if(v34 < rect.getMaxX() && v35 < rect.getMaxX()) {
			noJump = false;
			goto DOSTUFF;
		}

		//noJump = true;

	DOSTUFF:
		// avoid issues with mini player
		float gamemodeSize = MBO(float, self, 0x7C8);
		float scaleFactor = gamemodeSize * 0.5 * self->_playerScale();
		float scaleFactorUndiv = gamemodeSize * self->_playerScale();

		float yDifference = 0;
		if(self->_isGravityFlipped()) {
			yDifference = pos.x - object->getPosition().x;
		}
		else {
			yDifference = object->getPosition().x - pos.x;
		}
		bool canChangePosition = yDifference > 5;

		bool groundCol = false;

		if(self->_isDartMode() && !isDblock) {
			goto KILLPLAYER;
		}

		/*
			TOP AND BOTTOM COLLISION
		*/
		if ((pos.x + scaleFactor - 6 > rect.getMinX()) && (point2.x + scaleFactor - 6 > rect.getMinX())) {
			// left side
			if (self->_isGravityFlipped()) {
				if(canChangePosition) {
					float newX = rect.getMaxX() + scaleFactor;
					self->setPosition(CCPoint(newX, self->getPosition().y));
				}

				if (self->isInNormalMode()) self->checkSnapJumpToObject(object);

				if(hitHead && !isRollMode) self->setYVelocity(0, 23);
				else {
					if(noJump) self->hitGroundNoJump(isFlipGravity);
					else self->hitGround(isFlipGravity);

					self->updateCollideBottom(rect.getMaxX(), idkSomeInt);
				}
			}
			else {
				self->updateCollideTop(rect.getMinX(), idkSomeInt);

				if (self->_isPlatformer())
					self->exitPlatformerAnimateJump();

				// MBO(int, self, 0x725) = 0;
				if (hitHead) {
					self->didHitHead();
					self->setYVelocity(0, 29);
				}
			}

			MBO(GameObject *, self, 0x548) = object;

			//MBO(int, self, 0xB00) = 0;
		}
		else {
			// right side
			if (!self->_isGravityFlipped()) {
				if(canChangePosition) {
					/*float waveAdd = 0;

					if(isDblock) {

					}*/

					float newX = rect.getMinX() - scaleFactor;
					self->setPosition(CCPoint(newX, self->getPosition().y));
				}
				
				if(self->isInNormalMode()) self->checkSnapJumpToObject(object);

				if(hitHead && !isRollMode) self->setYVelocity(0, 23);
				else {
					if(noJump) self->hitGroundNoJump(isFlipGravity);
					else self->hitGround(isFlipGravity);

					self->updateCollideBottom(rect.getMinX(), idkSomeInt);
				}
			}
			else {
				self->updateCollideTop(rect.getMaxX(), idkSomeInt);

				if (self->_isPlatformer()) {
					self->exitPlatformerAnimateJump();
				}

				if(hitHead) {
					self->didHitHead();
					self->setYVelocity(0, 29);
				}
			}

			MBO(GameObject *, self, 0x548) = object;

			//MBO(int, self, 0xB00) = 0;
		}

		/*
			LEFT AND RIGHT COLLISION
		*/
		if(self->_isPlatformer()) {
			float v65 = 0;
			float v66 = 0;
			if(MBO(bool, self, 0x748) || MBO(GameObject*, self, 0x749) != nullptr || MBO(double, self, 0x7B8) - MBO(double, self, 0xB50) < 0.200000003) {
				v65 = pos.x - (gamemodeSize * 0.5) * self->flipMod();
				v66 = pos.x;
			}
			else {
				v65 = pos.x + gamemodeSize * 0.5 - mul1;
				v66 = pos.x - gamemodeSize * 0.5 + mul1;
			}

			auto objPos = object->getPosition();

			// make sure its wall collision
			//CCLog("v65: %f, v66: %f, rect: (MinX: %f, MaxX: %f, MinY: %f, MaxY: %f)", v65, v66, rect.getMinX(), rect.getMaxX(), rect.getMinY(), rect.getMaxY());
			if(v65 < rect.getMaxX() && v65 > rect.getMinX() || v66 > rect.getMaxY() && v66 < rect.getMinY()
				&& CCRect(objPos.x + 2.5, objPos.y, scaleFactorUndiv, gamemodeSize - 5).intersectsRect(rect)) {
				//CCLog("why are you");
				if(pos.y + scaleFactor - 6 > rect.getMinY() && point2.y + scaleFactor - 6 > rect.getMinY()) {
					// left
					if(pos.x - scaleFactor + 6 < rect.getMaxY() && point2.x - scaleFactor + 6 < rect.getMaxY()) {
						if(isDblock) return;
					}

					self->updateCollideLeft(rect.getMaxY(), idkSomeInt);

					float newY = rect.getMaxY() + scaleFactor;
					self->setPosition(CCPoint(pos.x, newY));
				}
				else {
					// right
					self->updateCollideRight(rect.getMinX(), idkSomeInt);

					float newY = rect.getMinY() - scaleFactor;
					//self->setPosition(CCPoint(pos.x, newY));
				}

				MBO(int, self, 0xB00) = 0;
				if(self->_isDashing()) {
					self->stopDashing();
					MBO(int, self, 0x725) = 0;
				}

				if(MBO(bool, self, 0xB5A)) MBO(int, self, 0xB3C) = 0;
			}
		}

		// kill the player
		if (!self->_isPlatformer()) {
		KILLPLAYER:
			if (rect.intersectsRect(playerRect)) {
				if (self->_inPlayLayer()) {
					GM->_playLayer()->destroyPlayer(self, nullptr);
				}
				else {
					CCLog("die");
				}
			}
		}
	}
	else {
		PlayerObject_collidedWithObjectInternalO(self, idk, obj, hitbox, what);

		// make ignore damage actually work
		if(GM->getGameVariable("0009")) MBO(bool, self, 0x724) = false;
	}
}

void CollisionFix::ApplyHooks() {
	HOOK("_ZN12PlayerObject26collidedWithObjectInternalEfP10GameObjectN7cocos2d6CCRectEb", PlayerObject_collidedWithObjectInternalH, PlayerObject_collidedWithObjectInternalO);
}