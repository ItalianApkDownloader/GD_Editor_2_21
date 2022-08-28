

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
-----------------------------------------------------

		This is some random backup of my previous code of 2p platformer
		Its super buggy so just idk heres it if you want to see
		I know that the code is super bad so im just starting from 0 lol
		(dont copy it lmao)
		
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
-----------------------------------------------------
	


	CCRect R_right_btn = {0, 0, 240, 100};
	CCRect R_left_btn = {0, 0, 240, 100};
	
	//	CCRect right_btn = {100, 0, 240, 100};
	//CCRect left_btn = {0, 0, 240, 100};
	
	bool p1Jumping;
	bool p2Jumping;
	
	// make platformer dpad visible
	#include "CCDrawNode.h"
	
FUNCTIONHOOK(void, PlayLayer_Update, GJBaseGameLayer* self, void* a1, void* a2, void* a3, void* a4) {
	
	PlayLayer_UpdateO(self, a1, a2, a3, a4);
	if(GM->_inEditor()) return ;
	
	auto uilayer = MBO(UILayer*, GM->_playLayer(), 0x2CA0);
	bool platformerBtns_visible = self->_isDual();
	
	if(uilayer->_platformer() && uilayer->isTwoPlayer()) {
	
		auto dpad = (CCSprite*)uilayer->getChildByTag(100);
		auto dpad_dwn = (CCSprite*)uilayer->getChildByTag(101);
	
		dpad->CCSprite::setOpacity(platformerBtns_visible ? 255 : 0);
		dpad_dwn->CCSprite::setOpacity(platformerBtns_visible ? 255 : 0);
	}
	
					
	
	
}
FUNCTIONHOOK(bool, UILayer_init, UILayer* self) {
	if(!UILayer_initO(self)) return false;
	

	
	if(!MEMBERBYOFFSET(bool, self, 0x206)) return true;
	
		auto dpad = MEMBERBYOFFSET(CCSprite*, self, 0x1D8);
		bool platformerBtns_visible = !GM->getGameVariable("10007");
		dpad->setVisible(platformerBtns_visible);

		auto node = cocos2d::CCDrawNode::create();
		cocos2d::CCPoint m_points[4];
	
				auto left_btn = self->_leftBtn();
				auto right_btn = self->_rightBtn();
		
		
				if(self->isTwoPlayer()) {

		
		auto dpadRight_Dwn = CCSprite::createWithSpriteFrameName("Dpad_Btn_Dwn.png"); //Left default!!
		dpadRight_Dwn->setPositionY(dpad->GPY());
		dpadRight_Dwn->setPositionX(CCRIGHT - (dpad->GPX() - CCLEFT));
		dpadRight_Dwn->setTag(101);
		dpadRight_Dwn->setVisible(false);
		self->addChild(dpadRight_Dwn);
		
				auto dpadRight = CCSprite::createWithSpriteFrameName("Dpad_Btn.png");
		dpadRight->setPositionY(dpad->GPY());
		dpadRight->setPositionX(CCRIGHT - (dpad->GPX() - CCLEFT));
		dpadRight->setTag(100);
		//dpadRight->setVisible(false);
		self->addChild(dpadRight);
		
		

				R_right_btn = right_btn;
				R_left_btn = left_btn;
		
				R_right_btn.origin.x += 525;
				R_left_btn.origin.x += 470;
				
				
			if(GM->getGameVariable("100009")) {
			

	
				m_points[0] = ccp(R_right_btn.getMinX(),R_right_btn.getMinY());
				m_points[1] = ccp(R_right_btn.getMaxX(),R_right_btn.getMinY());
				m_points[2] = ccp(R_right_btn.getMaxX(),R_right_btn.getMaxY());
				m_points[3] = ccp(R_right_btn.getMinX(),R_right_btn.getMaxY());
				node->drawPolygon(m_points, 4, ccc4f(0, 0, 0, 0), 1, ccc4f(1, 1, 0, 1));
	
				m_points[0] = ccp(R_left_btn.getMinX(),R_left_btn.getMinY());
				m_points[1] = ccp(R_left_btn.getMaxX(),R_left_btn.getMinY());
				m_points[2] = ccp(R_left_btn.getMaxX(),R_left_btn.getMaxY());
				m_points[3] = ccp(R_left_btn.getMinX(),R_left_btn.getMaxY());
				node->drawPolygon(m_points, 4, ccc4f(0, 0, 0, 0), 1, ccc4f(1, 0, 0, 1));
			}
		
		}
		
	if(GM->getGameVariable("100009")) {

	m_points[0] = ccp(left_btn.getMinX(),left_btn.getMinY());
	m_points[1] = ccp(left_btn.getMaxX(),left_btn.getMinY());
	m_points[2] = ccp(left_btn.getMaxX(),left_btn.getMaxY());
	m_points[3] = ccp(left_btn.getMinX(),left_btn.getMaxY());
	node->drawPolygon(m_points, 4, ccc4f(0, 0, 0, 0), 1, ccc4f(1, 0, 0, 1));
	
	m_points[0] = ccp(right_btn.getMinX(),right_btn.getMinY());
	m_points[1] = ccp(right_btn.getMaxX(),right_btn.getMinY());
	m_points[2] = ccp(right_btn.getMaxX(),right_btn.getMaxY());
	m_points[3] = ccp(right_btn.getMinX(),right_btn.getMaxY());
	node->drawPolygon(m_points, 4, ccc4f(0, 0, 0, 0), 1, ccc4f(1, 1, 0, 1));
	
	self->addChild(node);
	}



	#ifdef SHADERDEBUG
	reinterpret_cast<UILayerDebug*>(self)->doInit();
	#endif

	return true;
}


bool(*UILayer_ccTouchBeganO)(UILayer *self, CCTouch *touch, CCEvent *event);
bool UILayer_ccTouchBeganH(UILayer *self, CCTouch *touch, CCEvent *event)
{
	CCLog("enter began");
	//bool ret = UILayer_ccTouchBeganO(self, touch, event);
						CCLog("touch->touch id: %d", touch->_touchID());

	// println("touch {} {}", touch->getID(), from < int>(touch, 0x30));

	bool is_platformer = self->_platformer();
	if (is_platformer)
	{
		auto &touch_id = self->_touchID();
		bool isDual = self->isDual();
		bool isTwoPlayer = self->isTwoPlayer();
		bool platformerBtns_visible = !GM->getGameVariable("10007");
		auto p2 = GM->_playLayer()->_player2();


			
		if (touch_id != -1) return UILayer_ccTouchBeganO(self, touch, event);

		auto touch_pos = touch->getLocation();
		touch_pos = self->convertToNodeSpace(touch_pos);

		// dont know if the order is right but doesnt matter
		auto left_btn = self->_leftBtn();
		auto right_btn = self->_rightBtn();
		
		bool leftDpad = !(!left_btn.containsPoint(touch_pos) && !right_btn.containsPoint(touch_pos));
		bool rightDpad = !(!R_left_btn.containsPoint(touch_pos) && !R_right_btn.containsPoint(touch_pos));


		
					if(!isDual && (!left_btn.containsPoint(touch_pos) && !right_btn.containsPoint(touch_pos))) {
		//		CCLog("enter is not dual");
					touch_id = touch->_touchID();

				GM->_playLayer()->queueButton(1, true, false);
				return UILayer_ccTouchBeganO(self, touch, event);
			}
			
			//if is not two player, dual doesnt matter

			
			if(isTwoPlayer) {
					auto dpad = (CCSprite*)self->getChildByTag(100);
					auto dpad_dwn = (CCSprite*)self->getChildByTag(101);
					
		//		CCLog("enter isTwoPlayer");
			
				if(CCRIGHT / 2 > touch_pos.x && !leftDpad) { 
			//					CCLog("aaaaaa");
							if(!p2Jumping) {
						
				GM->_playLayer()->_player2()->pushButton(Jump); //left side
				p2Jumping = true;

								return UILayer_ccTouchBeganO(self, touch, event);

							}

				}
				else if(CCRIGHT / 2 < touch_pos.x && !rightDpad) { 
			//					CCLog("bbbb");
							if(!p1Jumping) {
				GM->_playLayer()->_player1()->pushButton(Jump); //right side
								return UILayer_ccTouchBeganO(self, touch, event);

				p1Jumping = true;
							}

				



				if(rightDpad) {


					if (R_left_btn.containsPoint(touch_pos))
					{
				CCLog("rightDpad");
					if(R_right_btn.origin.x > touch_pos.x) {
					CCLog("rightDpad Left");
					p2->pushButton(Left);
					
						if(platformerBtns_visible) {
							dpad->CCSprite::setOpacity(0);
							dpad_dwn->CCSprite::setOpacity(255);
							dpad_dwn->setFlipX(false);
							dpad->CCSprite::setVisible(true);
							
						}
					
					} else if(R_right_btn.origin.x < touch_pos.x) {
					CCLog("rightDpad Right");
					p2->pushButton(Right);
					
						if(platformerBtns_visible) {
							dpad->CCSprite::setOpacity(0);
							dpad->CCSprite::setVisible(true);

							dpad_dwn->CCSprite::setOpacity(255);
							dpad_dwn->setFlipX(true);
						}
					
					
					}
			
				}
		
			}
			}
		}

			
	}
	
	return UILayer_ccTouchBeganO(self, touch, event);;
}

void(*UILayer_ccTouchEndedO)(UILayer *self, CCTouch *touch, CCEvent *event);
void UILayer_ccTouchEnded(UILayer *self, CCTouch *touch, CCEvent *event)
{
	CCLog("enter end");
	UILayer_ccTouchEndedO(self, touch, event);

	bool is_platformer = self->_platformer();
	auto &touch_id = self->_touchID();
	bool isDual = self->isDual();
	bool isTwoPlayer = self->isTwoPlayer();
	bool platformerBtns_visible = !GM->getGameVariable("10007");

	auto left_btn = self->_leftBtn();
	auto right_btn = self->_rightBtn();
		
	auto touch_pos = touch->getLocation();
	touch_pos = self->convertToNodeSpace(touch_pos);
		
			if(is_platformer && !isDual && (!left_btn.containsPoint(touch_pos) && !right_btn.containsPoint(touch_pos))) {
				touch_id = -1;
				GM->_playLayer()->queueButton(1, false, false);
				return UILayer_ccTouchEndedO(self, touch, event);
			}
			
			
	if (is_platformer && (touch_id == touch->_touchID() || (isTwoPlayer && isDual)))
	{


		auto p2 = GM->_playLayer()->_player2();
		



		bool leftDpad = left_btn.containsPoint(touch_pos) || right_btn.containsPoint(touch_pos);
		bool rightDpad = R_left_btn.containsPoint(touch_pos) || R_right_btn.containsPoint(touch_pos);
	

			
			if(isTwoPlayer) {
	//		CCLog("enter isTwoPlayer");
	
						auto dpad = (CCSprite*)self->getChildByTag(100);
					auto dpad_dwn = (CCSprite*)self->getChildByTag(101);
					
					
				if(CCRIGHT / 2 > touch_pos.x && !leftDpad) {
							touch_id = -5;

	//						CCLog("aaaaaa");
	
				GM->_playLayer()->_player2()->releaseButton(Jump); //left side
				

								return UILayer_ccTouchEndedO(self, touch, event);

							}

				
				else if(CCRIGHT / 2 < touch_pos.x && !rightDpad) {
							touch_id = -6;

				//				CCLog("bbbb");
				GM->_playLayer()->_player1()->releaseButton(Jump); //right side
								return UILayer_ccTouchEndedO(self, touch, event);

				}

				


				//this doesnt work yet
				if(rightDpad) {
				

					
					if (R_left_btn.containsPoint(touch_pos))
					{
				CCLog("rightDpad");
					if(R_right_btn.origin.x > touch_pos.x) {
					CCLog("rightDpad Left");
					
					p2->releaseButton(Left);
					
						if(platformerBtns_visible) {
							dpad->CCSprite::setOpacity(255);
							dpad->CCSprite::setVisible(true);
							
							dpad_dwn->CCSprite::setOpacity(0);
						}
					
					
					} else if(R_right_btn.origin.x < touch_pos.x) {
					CCLog("rightDpad Right");
					p2->releaseButton(Right);
					
								if(platformerBtns_visible) {
								CCLog("dpad visible false");
															dpad->CCSprite::setVisible(true);

									dpad->CCSprite::setOpacity(255);
									dpad_dwn->CCSprite::setOpacity(0);

								}
						
					}
			
				}
		
			}
			
				
		
			}
			
	}
	

	
	
	}



void(*UILayer_ccTouchMovedO)(UILayer *self, CCTouch *touch, CCEvent *event);
void UILayer_ccTouchMoved(UILayer *self, CCTouch *touch, CCEvent *event)
{
	CCLog("enter moved");
	UILayer_ccTouchMovedO(self, touch, event);

	auto &touch_id = self->_touchID();
	bool is_platformer = self->_platformer();
	bool isDual = self->isDual();
	bool isTwoPlayer = self->isTwoPlayer();
	bool platformerBtns_visible = !GM->getGameVariable("10007");
			
		if(!isDual || !isTwoPlayer) return;
		CCLog("continue");

	if (is_platformer && (touch_id == touch->_touchID() ||  (isTwoPlayer && isDual)))
	{
			auto p2 = GM->_playLayer()->_player2();
			auto p1 = GM->_playLayer()->_player1();
		
				auto left_btn = self->_leftBtn();
		auto right_btn = self->_rightBtn();
				auto touch_pos = touch->getLocation();
		touch_pos = self->convertToNodeSpace(touch_pos);

		bool leftDpad = left_btn.containsPoint(touch_pos) || right_btn.containsPoint(touch_pos);
		bool rightDpad = R_left_btn.containsPoint(touch_pos) || R_right_btn.containsPoint(touch_pos);

			
			if(isTwoPlayer) {
			
								auto dpad = (CCSprite*)self->getChildByTag(100);
					auto dpad_dwn = (CCSprite*)self->getChildByTag(101);
					
					
				if(CCRIGHT / 2 > touch_pos.x && !leftDpad) {

	//						CCLog("aaaaaa");
				GM->_playLayer()->_player2()->pushButton(Jump); //left side
				GM->_playLayer()->_player1()->releaseButton(Jump); //left side
				return UILayer_ccTouchMovedO(self, touch, event);

							}

				
				else if(CCRIGHT / 2 < touch_pos.x && !rightDpad) {

				//				CCLog("bbbb");
				GM->_playLayer()->_player1()->pushButton(Jump); //right side
				GM->_playLayer()->_player2()->releaseButton(Jump); //right side
								return UILayer_ccTouchMovedO(self, touch, event);

				}
			

				


				//this doesnt work yet
				if(rightDpad) {
				

					if (R_left_btn.containsPoint(touch_pos))
					{
				CCLog("rightDpad");
					if(R_right_btn.origin.x > touch_pos.x) {
					CCLog("rightDpad Left");
					p2->releaseButton(Right);
					p2->pushButton(Left);
					p1->releaseButton(Jump);
					
						if(platformerBtns_visible) {
							dpad->CCSprite::setOpacity(0);
							dpad_dwn->CCSprite::setOpacity(255);
							dpad_dwn->setFlipX(false);
						}
					
				
					} else if(R_right_btn.origin.x < touch_pos.x) 
					{
						CCLog("rightDpad Right");
						p2->releaseButton(Left);
						p2->pushButton(Right);
						p1->releaseButton(Jump);

							if(platformerBtns_visible) {
								dpad->CCSprite::setOpacity(0);
								dpad_dwn->CCSprite::setOpacity(255);
								dpad_dwn->setFlipX(true);
					}
					}   
			
				}
		
			}
	
		}
	}
}