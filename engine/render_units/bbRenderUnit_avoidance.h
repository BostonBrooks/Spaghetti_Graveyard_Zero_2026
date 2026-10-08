///Render units are groups of 12 units that are modelled in the ECS as one entity.
///The positions of render units are calculated based off the position of the group and an offset.
///Render units from one group clip through render units of anither group.
///Here we introduce a corrective force to prevent units from occupying the same space,


#include "bbRenderUnits.h"

bbFlag bbRenderUnits_avoidance(bbRenderUnits* render_units);
