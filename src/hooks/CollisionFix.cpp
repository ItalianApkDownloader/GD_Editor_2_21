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
FUNCTIONHOOK(void, PlayerObject_collidedWithObjectInternal, PlayerObject* self, float a2, GameObject* obj, CCRect hitbox, bool what) {
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
		if(self->isFlying() && !self->_isPlatformer()) mul2 = self->flipMod() * 6; // funky rob hardcode
		
		if(MBO(GameObject*, self, 0x749) != nullptr) {
			mul2 = mul2 + MBO(double, self, 0x578) * self->flipMod();
		}
		if(object != nullptr) {
			isPassableObj = MBO(bool, object, 0x4AE);
		}

		if(isPassableObj) return; // we dont want any collision action with passable objects

		//CCLog("unkvariable: %f", idk);
		/*if(obj != nullptr) {
			if(a2 > 0) {
				auto lastPos = obj->getLastPosition();
				auto realPos = obj->getRealPosition();
				auto scaleD = obj->getScalePosDelta();

				MBO(double, self, 0x550) = 0.054 / a2 * self->flipMod(); // this is a risky hardcore
			}
		}*/

		//CCLog("0x46C: %f", MBO(float, self, 0x46C));
		float v31 = MBO(float, self, 0x7C8) * 0.5 * self->_playerScale();
		float v32 = pos.x + v31 * self->flipMod();
		float v34 = v32 - mul2;
		float v35 = pos.x + v31 * self->flipMod() - mul2;

		// H blocks and platformer
		bool hitHead = false;
		if(MBO(int, self, 0xB7C) > 0 || self->_isPlatformer() || MBO(int, self, 0xB80) > 0) {
			hitHead = true;
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

		//CCLog("v34: %f, v35: %f", v34, v35);
		//CCLog("MinX: %f; MaxX: %f; MinY: %f; MaxY: %f;", rect.getMinX(), rect.getMaxX(), rect.getMinY(), rect.getMaxY());
		fmtlog("1 | noJump: {}: isFLying: {}", noJump, self->isFlying());
		if(self->_isGravityFlipped()) {
			fmtlog("5 | noJump: {}: isFLying: {}", noJump, self->isFlying());
			if(v34 > rect.getMinX() && v35 > rect.getMinX()) {
				
				noJump = false;
				fmtlog("2 | noJump: false isFLying: {}", self->isFlying());

				goto DOSTUFF;
			}
		}
		else if(v34 < rect.getMaxX() && v35 < rect.getMaxX()) {
			noJump = false;
			fmtlog("3 | noJump: false isFLying: {}", self->isFlying());
			goto DOSTUFF;
		}
		else if(v34 < rect.getMinY() && v35 < rect.getMinY()) {
			noJump = false;
			fmtlog("4 | noJump: false isFLying: {}", self->isFlying());
			goto DOSTUFF;
		}
		else {
		CCLog("YOU BUGGED OR YOU GONNA DIE VERY SOON");
		//according to my testing
		//this else only gets executed when you are very close to dying 
		//OR when the actual arrow bug happens
		//and this seems to fix it (not 100% sure)
		noJump = false;
		}

	DOSTUFF:
		// avoid issues with mini player
		float gamemodeSize = MBO(float, self, 0x7C8);
		float scaleFactor = gamemodeSize * 0.5 * self->_playerScale();
		float scaleFactorUndiv = gamemodeSize * self->_playerScale();

		float yDifference = 0;
		if(self->_isGravityFlipped()) {
			yDifference = pos.x - rect.origin.x;
		}
		else {
			yDifference = rect.origin.x - pos.x;
		}

		//CCLog("yDifference: %f", yDifference);
		bool canChangePosition = fabs(yDifference) > 5 * self->_playerScale();
		//CCLog("canChangePosition: %i", canChangePosition);

		if(self->_isDartMode() && !isDblock) {
			goto KILLPLAYER;
		}

		/*
			LEFT AND RIGHT COLLISION
		*/
		if(self->_isPlatformer()) {
			float v65 = 0;
			float v66 = 0;
			if(MBO(GameObject*, self, 0x748) != nullptr || MBO(GameObject*, self, 0x749) != nullptr || MBO(double, self, 0x7B8) - MBO(double, self, 0xB50) < 0.200000003) {
				v65 = pos.x - scaleFactor * self->flipMod();
				v66 = pos.x;
			}
			else {
				v65 = pos.x - scaleFactor + mul1;
				//v66 = pos.x + scaleFactor - mul1;
			}
			
			// make sure its wall collision
			//CCLog("v65: %f, v66: %f, rect: (MinX: %f, MaxX: %f, MinY: %f, MaxY: %f)", v65, v66, rect.getMinX(), rect.getMaxX(), rect.getMinY(), rect.getMaxY());
			//if(v65 < rect.getMaxX() && v65 > rect.getMinX() || v66 < rect.getMaxX() && v66 > rect.getMinX()
			if(v65 > rect.getMinX() && v65 < rect.getMaxX() //|| v66 < rect.getMaxX() && v66 > rect.getMinX()
				&& CCRect(playerRect2.origin.x - 2.5, playerRect2.origin.y, gamemodeSize - 5, scaleFactorUndiv).intersectsRect(rect)) {

				//CCLog("xVelocity: %f", self->_xVelocityPlatformer());

				int v76 = 0;
				if(pos.y + scaleFactor - 6 > rect.getMinY() && point2.y + scaleFactor - 6 > rect.getMinY()) {
					// left
					//CCLog("left wall");
					if(pos.x - scaleFactor + 6 < rect.getMaxY() && point2.x - scaleFactor + 6 < rect.getMaxY()) {
						// not sure what this is
						//if(isDblock) return;
					}

					self->updateCollideLeft(rect.getMaxY(), idkSomeInt);

					float newY = rect.getMaxY() + scaleFactor;
					self->setPosition(CCPoint(pos.x, newY));

					v76 = MBO(int, self, 0xB59);
				}
				else {
					// right
					//CCLog("right wall");
					self->updateCollideRight(rect.getMinX(), idkSomeInt);

					float newY = rect.getMinY() - scaleFactor;
					self->setPosition(CCPoint(pos.x, newY));

					v76 = MBO(int, self, 0xB5A);
				}

				self->_xVelocityPlatformer() = 0;

				if(self->_isDashing()) {
					self->stopDashing();
					MBO(int, self, 0x725) = 0;
				}

				//if(v76) MBO(int, self, 0xB3C) = 0;

				//if(MBO(bool, self, 0xB5A)) MBO(int, self, 0xB3C) = 0;

				goto FINISH;
			}
			else {
				goto TBCOLLISION;
			}
		}

		/*
			TOP AND BOTTOM COLLISION
		*/
	TBCOLLISION:
		// get new position
		pos = self->getPosition();

		// avoid wall collision mumbo jumbo
		//CCLog("TB collision");
		if(CCRect(playerRect2.origin.x, playerRect2.origin.y + 2.5, playerRect2.size.width, scaleFactorUndiv - 5).intersectsRect(rect)) {
		//	CCLog("TB collision yessssssssssssssssssssssssssssss");
			if ((pos.x + scaleFactor - 6 > rect.getMinX()) && (point2.x + scaleFactor - 6 > rect.getMinX())) {
			//if(!(v34 <= rect.getMinX() || v35 <= rect.getMinX())) {
				//CCLog("collided");
				// left side
				if (self->_isGravityFlipped()) {
					//CCLog("left side: bottom collided");
					if(self->_yVelocity() > 0) {
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
							if(obj != nullptr) MBO(GameObject*, self, 0x548) = obj; 
						}
					}
				}
				else {
					//CCLog("left side: top collided");
					self->updateCollideTop(rect.getMinX(), idkSomeInt);

					if (self->_isPlatformer())
						self->exitPlatformerAnimateJump();

					// MBO(int, self, 0x725) = 0;
					if (hitHead) {
						self->didHitHead();
						self->setYVelocity(0, 29);
					}
				}

				//MBO(GameObject *, self, 0x548) = object;

				//MBO(int, self, 0xB00) = 0;
				//goto FINISH;
			}
			else {
				// right side
				if (!self->_isGravityFlipped()) {
					//CCLog("right side: bottom collided");
					if(self->_yVelocity() <= 0) {
						if(canChangePosition) {
							//CCLog("pos set");
							float newX = rect.getMinX() - scaleFactor;
							self->setPosition(CCPoint(newX, self->getPosition().y));
						}
						
						if(self->isInNormalMode()) self->checkSnapJumpToObject(object);

						if(hitHead && !isRollMode) self->setYVelocity(0, 23);
						else {
							if(noJump) self->hitGroundNoJump(isFlipGravity);
							else self->hitGround(isFlipGravity);

							self->updateCollideBottom(rect.getMinX(), idkSomeInt);
							if(obj != nullptr) MBO(GameObject*, self, 0x548) = obj;
						}
					}
				}
				else {
					//CCLog("right side: top collided");
					self->updateCollideTop(rect.getMaxX(), idkSomeInt);

					if (self->_isPlatformer()) {
						self->exitPlatformerAnimateJump();
					}

					if(hitHead) {
						self->didHitHead();
						self->setYVelocity(0, 29);
					}
				}

				//MBO(GameObject *, self, 0x548) = object;

				//MBO(int, self, 0xB00) = 0;
			}
		}

	FINISH:
		// kill the player
		if (rect.intersectsRect(playerRect)) {
			/*if(noJump && self->isSafeFlip(0.1)) {
				CCLog("set bool");
				MBO(bool, self, 0x790) = false;
			}*/

			if (!self->_isPlatformer()) {
			KILLPLAYER:
				if (self->_inPlayLayer()) {
					GM->_playLayer()->destroyPlayer(self, nullptr);
					return;
				}
				else {
					MBO(bool, self, 0x724) = true; // shouldBeDead
					return;
				}
			}
		}
	}
	else {
		PlayerObject_collidedWithObjectInternalO(self, a2, obj, hitbox, what);

		// make ignore damage actually work
		if(GM->getGameVariable("0009")) MBO(bool, self, 0x724) = false;
	}
}

void CollisionFix::ApplyHooks() {
	HOOK("_ZN12PlayerObject26collidedWithObjectInternalEfP10GameObjectN7cocos2d6CCRectEb", PlayerObject_collidedWithObjectInternalH, PlayerObject_collidedWithObjectInternalO);
}