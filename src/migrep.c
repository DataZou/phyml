/*

PhyML:  a program that  computes maximum likelihood phylogenies from
DNA or AA homoLOGous sequences.

Copyright (C) Stephane Guindon. Oct 2003 onward.

All parts of the source except where indicated are distributed under
the GNU public licence. See http://www.opensource.org for details.

*/

/* Routines that implement Etheridge and Barton's model of continuous-space
   coalescent.
*/

#include "migrep.h"

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

int MIGREP_Main(int argc, char *argv[])
{
  t_tree *tree;
  phydbl *res;
  int seed,pid,burnin;
  FILE *fp_out;
  char *s;
  t_dsk *disk;

  s = (char *)mCalloc(T_MAX_NAME,sizeof(char));

  pid = getpid();
  seed = pid;
  /* seed = 25921; */
  /* seed = 12965; */
  /* seed = 2647; */
  /* seed = 13824; */

  printf("\n. seed: %d",seed);
  srand(seed);
  
  strcpy(s,"migrep.out");
  sprintf(s+strlen(s),".%d",pid);
  fp_out = Openfile(s,WRITE);
  /* fp_out = stderr; */

  setvbuf(fp_out,NULL,_IOFBF,1024);


  tree = MIGREP_Simulate_Backward((int)atoi(argv[1]),(int)atoi(argv[2]),10.,10.,seed);

  disk = tree->disk;
  while(disk->prev) disk = disk->prev;


  /* PhyML_Fprintf(fp_out,"%f %f %f %f %f\n", */
  /*              tree->mmod->lbda, */
  /*              tree->mmod->mu, */
  /*              tree->mmod->rad, */
  /*              2./tree->mmod->mu, */
  /*              MIGREP_Neigborhood_Size_Regression(tree)); */
  /* fclose(fp_out); */
  /* Exit("\n"); */



  PhyML_Fprintf(fp_out,"\n# SampArea\t TrueLbda\t TrueMu\t TrueRad\t TrueDist\t TrueNeigh\t RegNeigh\t TrueXroot\t TrueYroot\t Lbda5\t Lbda50\t Lbda95\t Mu5\t Mu50\t Mu95\t Rad5\t Rad50\t Rad95\t Xroot5\t Xroot50\t Xroot95\t Yroot5\t Yroot50\t Yroot95\t limXroot5\t limXroot50\t limXroot95\t limYroot5\t limYroot50\t limYroot95\t ");

  PhyML_Fprintf(fp_out,"\n %f\t %f\t %f\t %f\t %f\t %f\t %f\t",
                tree->mmod->sampl_area,
                tree->mmod->lbda,
                tree->mmod->mu,
                tree->mmod->rad,
                MIGREP_Dist_Parent_To_Offspring(tree),
                MIGREP_Neigborhood_Size(tree),
                MIGREP_Neigborhood_Size_Regression(tree),
                disk->ldsk->coord->lonlat[0],
                disk->ldsk->coord->lonlat[1]);

  res = MIGREP_MCMC(tree);

  burnin = (int)(0.5*(tree->mcmc->run / tree->mcmc->sample_interval));

  PhyML_Fprintf(fp_out,"%f\t %f\t %f\t",
                /* Lbda5 */ Quantile(res+0*tree->mcmc->chain_len / tree->mcmc->sample_interval+burnin,tree->mcmc->run / tree->mcmc->sample_interval+1-burnin,0.025),
                /* Lbda50*/ Quantile(res+0*tree->mcmc->chain_len / tree->mcmc->sample_interval+burnin,tree->mcmc->run / tree->mcmc->sample_interval+1-burnin,0.50),
                /* Lbda95*/ Quantile(res+0*tree->mcmc->chain_len / tree->mcmc->sample_interval+burnin,tree->mcmc->run / tree->mcmc->sample_interval+1-burnin,0.975));

  PhyML_Fprintf(fp_out,"%f\t %f\t %f\t",
                /* mu5 */ Quantile(res+1*tree->mcmc->chain_len / tree->mcmc->sample_interval+burnin,tree->mcmc->run / tree->mcmc->sample_interval+1-burnin,0.025),
                /* mu50*/ Quantile(res+1*tree->mcmc->chain_len / tree->mcmc->sample_interval+burnin,tree->mcmc->run / tree->mcmc->sample_interval+1-burnin,0.50),
                /* mu95*/ Quantile(res+1*tree->mcmc->chain_len / tree->mcmc->sample_interval+burnin,tree->mcmc->run / tree->mcmc->sample_interval+1-burnin,0.975));

  PhyML_Fprintf(fp_out,"%f\t %f\t %f\t",
                /* rad5 */ Quantile(res+2*tree->mcmc->chain_len / tree->mcmc->sample_interval+burnin,tree->mcmc->run / tree->mcmc->sample_interval+1-burnin,0.025),
                /* rad50*/ Quantile(res+2*tree->mcmc->chain_len / tree->mcmc->sample_interval+burnin,tree->mcmc->run / tree->mcmc->sample_interval+1-burnin,0.50),
                /* rad95*/ Quantile(res+2*tree->mcmc->chain_len / tree->mcmc->sample_interval+burnin,tree->mcmc->run / tree->mcmc->sample_interval+1-burnin,0.975));

  PhyML_Fprintf(fp_out,"%f\t %f\t %f\t",
                /* X5 */ Quantile(res+3*tree->mcmc->chain_len / tree->mcmc->sample_interval+burnin,tree->mcmc->run / tree->mcmc->sample_interval+1-burnin,0.025),
                /* X50*/ Quantile(res+3*tree->mcmc->chain_len / tree->mcmc->sample_interval+burnin,tree->mcmc->run / tree->mcmc->sample_interval+1-burnin,0.50),
                /* X95*/ Quantile(res+3*tree->mcmc->chain_len / tree->mcmc->sample_interval+burnin,tree->mcmc->run / tree->mcmc->sample_interval+1-burnin,0.975));

  PhyML_Fprintf(fp_out,"%f\t %f\t %f\t",
                /* Y5 */ Quantile(res+4*tree->mcmc->chain_len / tree->mcmc->sample_interval+burnin,tree->mcmc->run / tree->mcmc->sample_interval+1-burnin,0.025),
                /* Y50*/ Quantile(res+4*tree->mcmc->chain_len / tree->mcmc->sample_interval+burnin,tree->mcmc->run / tree->mcmc->sample_interval+1-burnin,0.50),
                /* Y95*/ Quantile(res+4*tree->mcmc->chain_len / tree->mcmc->sample_interval+burnin,tree->mcmc->run / tree->mcmc->sample_interval+1-burnin,0.975));

  PhyML_Fprintf(fp_out,"%f\t %f\t %f\t",
                /* Dist5 */ Quantile(res+5*tree->mcmc->chain_len / tree->mcmc->sample_interval+burnin,tree->mcmc->run / tree->mcmc->sample_interval+1-burnin,0.025),
                /* Dist50*/ Quantile(res+5*tree->mcmc->chain_len / tree->mcmc->sample_interval+burnin,tree->mcmc->run / tree->mcmc->sample_interval+1-burnin,0.50),
                /* Dist95*/ Quantile(res+5*tree->mcmc->chain_len / tree->mcmc->sample_interval+burnin,tree->mcmc->run / tree->mcmc->sample_interval+1-burnin,0.975));

  PhyML_Fprintf(fp_out,"%f\t %f\t %f\t",
                /* Neigh5 */ Quantile(res+6*tree->mcmc->chain_len / tree->mcmc->sample_interval+burnin,tree->mcmc->run / tree->mcmc->sample_interval+1-burnin,0.025),
                /* Neigh50*/ Quantile(res+6*tree->mcmc->chain_len / tree->mcmc->sample_interval+burnin,tree->mcmc->run / tree->mcmc->sample_interval+1-burnin,0.50),
                /* Neigh95*/ Quantile(res+6*tree->mcmc->chain_len / tree->mcmc->sample_interval+burnin,tree->mcmc->run / tree->mcmc->sample_interval+1-burnin,0.975));

  Free(res);  
  fclose(fp_out);

  return 0;
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
// Simulate Etheridge-Barton model backwards in time, following n_otu lineages
// on a rectangle of dimension width x height
// See Kelleher, Barton & Etheridge, Bioinformatics, 2013.
t_tree *MIGREP_Simulate_Backward(int n_otu, int n_sites, phydbl width, phydbl height, int r_seed)
{  
  t_tree *tree;
  int n_dim;
  t_migrep_mod *mmod;
  t_dsk *disk;
  option *io;
  t_mod *mod;
  t_opt *s_opt;
  calign *cdata;
  
  n_dim = 2; // 2-dimensional landscape

  io    = (option *)Make_Input();
  mod   = (t_mod *)Make_Model_Basic();
  s_opt = (t_opt *)Make_Optimiz();

  Set_Defaults_Input(io);
  Set_Defaults_Model(mod);
  Set_Defaults_Optimiz(s_opt);

  io->mod      = mod;
  mod->io      = io;
  mod->s_opt   = s_opt;
  io->r_seed   = r_seed;

  io->n_otu    = n_otu;
  io->init_len = 500; /* sequence length */

  io->data = Make_Empty_Alignment(io);
  /* Print_Seq(stdout,io->data,io->n_otu); */

  Make_Model_Complete(io->mod);
  Set_Model_Name(io->mod);

  /* Print_Settings(io); */

  io->colalias = NO;
  cdata = Compact_Data(io->data,io);
  Free_Seq(io->data,io->n_otu);

  tree = Make_Tree_From_Scratch(n_otu,cdata);

  Connect_CSeqs_To_Nodes(cdata,io,tree);
  
  tree->rates = RATES_Make_Rate_Struct(tree->n_otu);
  RATES_Init_Rate_Struct(tree->rates,io->rates,tree->n_otu);
  
  tree->data      = cdata;
  tree->mod       = mod;
  tree->io        = io;
  tree->n_pattern = tree->data->crunch_len;


  /* Allocate migrep model */
  mmod = MIGREP_Make_Migrep_Model(n_dim);
  MIGREP_Init_Migrep_Mod(mmod,n_dim);
  mmod->lim->lonlat[0] = width;
  mmod->lim->lonlat[1] = height;
  
  /* Initialize parameters of migrep model */
  mmod->lbda = Uni()*(0.3 - 0.05) + 0.05;
  mmod->mu   = Uni()*(1.0 - 0.3)  + 0.3;
  mmod->rad  = Uni()*(5.0 - 1.5)  + 1.5;

  /* mmod->lbda = 0.80; */
  /* mmod->mu   = 0.10; */
  /* mmod->rad  = 2.50; */

  /* mmod->rho  = 100.; */
  /* mmod->lbda = 0.1; */
  /* mmod->mu   = 0.4; */
  /* mmod->rad  = 2.0; */

  tree->mmod = mmod;
    
  /* MIGREP_Simulate_Backward_Core(YES,tree); */
  mmod->sampl_area = MIGREP_Simulate_Forward_Core(n_sites,tree);

  MIGREP_Ldsk_To_Tree(tree);  

  Update_Ancestors(tree->n_root,tree->n_root->v[2],tree);
  Update_Ancestors(tree->n_root,tree->n_root->v[1],tree);
  RATES_Fill_Lca_Table(tree);

  disk = tree->disk;
  while(disk->prev) disk = disk->prev;
  
  tree->rates->bl_from_rt = YES;
  tree->rates->clock_r    = 0.1 / FABS(disk->time);
  tree->rates->model      = STRICTCLOCK;
  RATES_Update_Cur_Bl(tree);

  Init_Model(cdata,mod,io);
  Prepare_Tree_For_Lk(tree);
  Evolve(tree->data,tree->mod,tree);
  
  if(tree->mod->s_opt->greedy) Init_P_Lk_Tips_Double(tree);
  else                         Init_P_Lk_Tips_Int(tree);
  Init_P_Lk_Loc(tree);

  return(tree);
}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/
// Simulate Etheridge-Barton model backwards in time, following n_otu lineages
// on a rectangle of dimension width x height
void MIGREP_Simulate_Backward_Core(int new_loc, t_tree *tree)
{
  t_dsk *disk;
  t_ldsk *new_ldsk,**ldsk_a,**ldsk_a_tmp;
  int i,j,n_disk,n_lineages,n_dim,n_hit,n_lineages_new,n_otu;
  phydbl dt_dsk,curr_t,prob_hit;
  t_migrep_mod *mmod;
  
  mmod  = tree->mmod;
  n_dim = tree->mmod->n_dim;
  n_otu = tree->n_otu;

  if(new_loc == YES)
    {
      disk = MIGREP_Make_Disk_Event(n_dim,n_otu);
      MIGREP_Init_Disk_Event(disk,n_dim,NULL);
      disk->time             = 0.0;
      disk->mmod             = mmod;
      disk->centr->lonlat[0] = .5*mmod->lim->lonlat[0];
      disk->centr->lonlat[1] = .5*mmod->lim->lonlat[1];      
      disk->n_ldsk_a         = n_otu;  
      tree->disk             = disk;
    }
  else
    {
      disk = tree->disk;
    }


  if(new_loc == YES)
    {
      /* Allocate coordinates for all the tips first (will grow afterwards) */
      ldsk_a = (t_ldsk **)mCalloc(n_otu,sizeof(t_ldsk *));
      For(i,n_otu) 
        {
          ldsk_a[i] = MIGREP_Make_Lindisk_Node(n_dim);
          MIGREP_Init_Lindisk_Node(ldsk_a[i],disk,n_dim);
        }
      
      /* Generate coordinates for the tip nodes (uniform distribution on the rectangle) */
      For(i,n_otu)
        {
          ldsk_a[i]->coord->lonlat[0] = Uni()*tree->mmod->lim->lonlat[0]; // longitude
          ldsk_a[i]->coord->lonlat[1] = Uni()*tree->mmod->lim->lonlat[1]; // latitude
        }
      
      disk->ldsk_a = ldsk_a;
    }
  else
    {
      ldsk_a = disk->ldsk_a;
    }


  /* Allocate and initialise for next event */
  disk->prev = MIGREP_Make_Disk_Event(n_dim,n_otu);
  MIGREP_Init_Disk_Event(disk->prev,n_dim,NULL);
  disk->prev->next = disk;

  /* Move to it */
  disk = disk->prev;

  ldsk_a_tmp = (t_ldsk **)mCalloc(n_otu,sizeof(t_ldsk *));

  curr_t     = 0.0;
  dt_dsk     = 0.0;
  n_lineages = tree->n_otu;
  n_disk     = 0;
  do
    {      
      /* Time of next event */
      dt_dsk = Rexp(mmod->lbda);
      curr_t -= dt_dsk;
      
      /* Coordinates of next event */
      disk->centr->lonlat[0] = Uni()*mmod->lim->lonlat[0];
      disk->centr->lonlat[1] = Uni()*mmod->lim->lonlat[1];      

      disk->time = curr_t;
      disk->mmod = mmod;

      /* printf("\n. Disk %s has %d lindisk nodes and %d disks under",disk->id,disk->n_ldsk_a,disk->n_disk_under); */

      /* New lindisk (will not be used if no lineage is hit)  */
      new_ldsk = MIGREP_Make_Lindisk_Node(n_dim);
      MIGREP_Init_Lindisk_Node(new_ldsk,disk,n_dim);

      /* Sample the location of new_ldsk. */
      /* Takes into account the limits of the landscape */
      switch(mmod->name)
        {
        case MIGREP_UNIFORM: { MIGREP_Runif_Rectangle_Overlap(new_ldsk,disk,mmod); break; }
        case MIGREP_NORMAL:  { MIGREP_Rnorm_Trunc(new_ldsk,disk,mmod); break; }
        }

      n_hit          = 0;
      n_lineages_new = 0;
      For(i,n_lineages)
        {
          ldsk_a[i]->prev = NULL;
          
          prob_hit = -1.;
          switch(mmod->name)
            {
            case MIGREP_UNIFORM: { prob_hit = mmod->mu; break; }
            case MIGREP_NORMAL:  
              { 
                prob_hit = LOG(mmod->mu);
                For(j,mmod->n_dim) prob_hit += -POW(ldsk_a[i]->coord->lonlat[j] - disk->centr->lonlat[j],2)/(2.*POW(mmod->rad,2));
                prob_hit = EXP(prob_hit);
                break; 
              }
            }
          
          if(MIGREP_Is_In_Disk(ldsk_a[i]->coord,disk,mmod) == YES) 
            {
              if(Uni() < prob_hit)
                {
                  /* printf("\n. Hit and die %s @ time %f. Center: %f %f Go to %f %f", */
                  /*        ldsk_a[i]->coord->id, */
                  /*        disk->time, */
                  /*        disk->centr->lonlat[0], */
                  /*        disk->centr->lonlat[1], */
                  /*        new_ldsk->coord->lonlat[0], */
                  /*        new_ldsk->coord->lonlat[1]); */
                  
                  MIGREP_Make_Lindisk_Next(new_ldsk);

                  ldsk_a[i]->prev                    = new_ldsk;
                  ldsk_a[i]->prev->is_hit            = YES;
                  new_ldsk->next[new_ldsk->n_next-1] = ldsk_a[i]; 
                  disk->ldsk                         = new_ldsk;

                  n_hit++;

                  if(n_hit == 1)
                    {
                      ldsk_a_tmp[n_lineages_new] = new_ldsk;
                      n_lineages_new++;
                    }
                }
            }
          
          if(ldsk_a[i]->prev == NULL)
            {
              ldsk_a[i]->prev = ldsk_a[i];
              ldsk_a_tmp[n_lineages_new] = ldsk_a[i]->prev;
              n_lineages_new++;
            }          
        }
      
      if((n_hit > 0) && (n_lineages_new != n_lineages - n_hit + 1))
        {
          PhyML_Printf("\n== Err. in file %s at line %d (function '%s') \n",__FILE__,__LINE__,__FUNCTION__);
          Warn_And_Exit("");
        }

      if(n_hit > 0) n_lineages -= (n_hit-1);
 
      ldsk_a = (t_ldsk **)mCalloc(n_otu,sizeof(t_ldsk *));
      For(i,n_lineages) ldsk_a[i] = ldsk_a_tmp[i];
      
      /* For(i,n_lineages) */
      /*   { */
      /*     printf("\n. disk %s [%15f] %3d %s %15f %15f", */
      /*            disk?disk->id:"xxx", */
      /*            disk?disk->time:0.0, */
      /*            i, */
      /*            ldsk_a[i]->coord->id, */
      /*            ldsk_a[i]->coord->lonlat[0], */
      /*            ldsk_a[i]->coord->lonlat[1]); */
      /*   } */

      n_disk++;


      if(n_lineages == 1) break;

      disk->prev = MIGREP_Make_Disk_Event(n_dim,n_otu);
      MIGREP_Init_Disk_Event(disk->prev,n_dim,NULL);
      disk->prev->next = disk;
      
      disk = disk->prev;
    }
  while(1);

  /* MIGREP_Print_Struct('#',tree); */

}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/
// Simulate Etheridge-Barton model forwards in time, following n_otu lineages
// on a rectangle of dimension width x height
phydbl MIGREP_Simulate_Forward_Core(int n_sites, t_tree *tree)
{
  t_dsk *disk;
  t_ldsk *new_ldsk,**ldsk_a_pop,**ldsk_a_samp,**ldsk_a_tmp,**ldsk_a_tips;
  int i,j,n_disk,n_dim,n_otu,pop_size,parent_id,n_lineages,sample_size,n_poly;
  phydbl dt_dsk,curr_t,sum,*parent_prob,prob_death,tree_height,max_x,max_y,trans_x,trans_y,area;
  short int dies,n_remain;
  t_migrep_mod *mmod;
  t_poly **poly;

  mmod     = tree->mmod;
  n_dim    = tree->mmod->n_dim;
  n_otu    = tree->n_otu;
  /* pop_size = (int)(tree->mmod->rho * tree->mmod->lim->lonlat[0] * tree->mmod->lim->lonlat[1]); */
  pop_size = 100*n_otu;

  parent_prob = (phydbl *)mCalloc(pop_size,sizeof(phydbl));

  /* Allocate and initialise first disk event */
  disk = MIGREP_Make_Disk_Event(n_dim,pop_size);
  MIGREP_Init_Disk_Event(disk,n_dim,NULL);

  /* Allocate coordinates for all the individuals in the population */
  ldsk_a_pop = (t_ldsk **)mCalloc(pop_size,sizeof(t_ldsk *));
  For(i,pop_size)
    {
      ldsk_a_pop[i] = MIGREP_Make_Lindisk_Node(n_dim);
      MIGREP_Init_Lindisk_Node(ldsk_a_pop[i],disk,n_dim);
    }

  /* Generate coordinates for all individuals */
  For(i,pop_size)
    {
      ldsk_a_pop[i]->coord->lonlat[0] = Uni()*tree->mmod->lim->lonlat[0]; // longitude
      ldsk_a_pop[i]->coord->lonlat[1] = Uni()*tree->mmod->lim->lonlat[1]; // latitude
    }

  disk->prev = NULL;
  curr_t = 0.0;
  dt_dsk = 0.0;
  n_disk = 0;
  do
    {
      /* Coordinates of event */
      disk->centr->lonlat[0] = Uni()*mmod->lim->lonlat[0];
      disk->centr->lonlat[1] = Uni()*mmod->lim->lonlat[1];

      /* Select one parent */
      For(i,pop_size)
        {
          switch(mmod->name)
            {
            case MIGREP_UNIFORM:
              {
                if(MIGREP_Is_In_Disk(ldsk_a_pop[i]->coord,disk,mmod) == YES)
                  parent_prob[i] = 1.0;
                else
                  parent_prob[i] = 0.0;
                break;
              }
            case MIGREP_NORMAL:
              {
                parent_prob[i] = 0.0;
                For(j,mmod->n_dim) parent_prob[i] += -POW(ldsk_a_pop[i]->coord->lonlat[j] - disk->centr->lonlat[j],2)/(2.*POW(mmod->rad,2));
                parent_prob[i] = EXP(parent_prob[i]);
                break;
              }
            }
        }
      sum = 0.0;
      For(i,pop_size) sum += parent_prob[i];
      For(i,pop_size) parent_prob[i] /= sum;
      
      parent_id  = Sample_i_With_Proba_pi(parent_prob,pop_size);
      disk->ldsk = ldsk_a_pop[parent_id];
      ldsk_a_pop[parent_id]->disk = disk;

      /* printf("\n. Disk %s has %s on it",disk->id,ldsk_a_pop[parent_id]->coord->id); */

      /* Which lineages die in that event? Each lineage that dies off is being replaced
         with a new one which location is chosen randomly following the model */
      For(i,pop_size)
        {
          prob_death = 0.0;
          switch(mmod->name)
            {
            case MIGREP_UNIFORM:
              {
                if(MIGREP_Is_In_Disk(ldsk_a_pop[i]->coord,disk,mmod) == YES)
                  prob_death = mmod->mu;
                break;
              }
            case MIGREP_NORMAL:
              {
                prob_death = LOG(mmod->mu);
                For(j,mmod->n_dim) prob_death += -POW(ldsk_a_pop[i]->coord->lonlat[j] - disk->centr->lonlat[j],2)/(2.*POW(mmod->rad,2));
                prob_death = EXP(prob_death);
                break;
              }
            }
          
          dies = NO;
          if(Uni() < prob_death || i == parent_id) dies = YES; /* Note: parent always dies (not in the model...) */

          if(dies == YES) /* Replace dead lineage with new one */
            {
              /* New lindisk */
              new_ldsk = MIGREP_Make_Lindisk_Node(n_dim);
              MIGREP_Init_Lindisk_Node(new_ldsk,disk,n_dim);
          
              /* Select new location for new lineage replacing the one that just died  */
              switch(mmod->name)
                {
                case MIGREP_UNIFORM: { MIGREP_Runif_Rectangle_Overlap(new_ldsk,disk,mmod); break; }
                case MIGREP_NORMAL:  { MIGREP_Rnorm_Trunc(new_ldsk,disk,mmod); break; }
                }

              /* Connect to parent */
              new_ldsk->prev = disk->ldsk;

              /* Replace dead individual (thus, number of birth == number of death) */
              ldsk_a_pop[i] = new_ldsk;

            }
        }

      /* if(!(n_disk%10)) { printf("\r n_disk: %8d",n_disk); fflush(NULL); } */

      disk->next = MIGREP_Make_Disk_Event(n_dim,n_otu);
      MIGREP_Init_Disk_Event(disk->next,n_dim,NULL);
      disk->next->prev = disk;      
      disk = disk->next;
      n_disk++;

      /* Time of next event */
      dt_dsk = Rexp(mmod->lbda);
      curr_t += dt_dsk;

      disk->time = curr_t;
      disk->mmod = mmod;
    }
  while(n_disk < 1000);

  For(i,pop_size) ldsk_a_pop[i]->disk = disk;

  /* Allocate coordinates for all the tips first (will grow afterwards) */
  ldsk_a_samp = (t_ldsk **)mCalloc(n_otu,sizeof(t_ldsk *));
  ldsk_a_tips = (t_ldsk **)mCalloc(n_otu,sizeof(t_ldsk *));
  ldsk_a_tmp  = (t_ldsk **)mCalloc(n_otu,sizeof(t_ldsk *));

  /* Sample individuals (take the first n_otu ldsk within ldsk_a_pop array) */  
  /* For(i,n_otu) ldsk_a_samp[i] = ldsk_a_pop[i]; */

  n_poly = n_sites;

  do
    {
      poly = (t_poly **)mCalloc(n_poly,sizeof(t_poly *));
      For(i,n_poly) poly[i] = Rpoly(3);
      For(i,n_poly)
        {      
          For(j,poly[i]->n_poly_vert) 
            {
              poly[i]->poly_vert[j]->lonlat[0] *= mmod->lim->lonlat[0]*0.5;
              poly[i]->poly_vert[j]->lonlat[1] *= mmod->lim->lonlat[1]*0.5;
            }
          
          max_x = 0.0;
          max_y = 0.0;
          For(j,poly[i]->n_poly_vert) 
            {
              if(poly[i]->poly_vert[j]->lonlat[0] > max_x) max_x = poly[i]->poly_vert[j]->lonlat[0];
              if(poly[i]->poly_vert[j]->lonlat[1] > max_y) max_y = poly[i]->poly_vert[j]->lonlat[1];
            }
          
          trans_x = Uni()*(mmod->lim->lonlat[0] - max_x);
          trans_y = Uni()*(mmod->lim->lonlat[1] - max_y);
          
          For(j,poly[i]->n_poly_vert) 
            {
              poly[i]->poly_vert[j]->lonlat[0] += trans_x;
              poly[i]->poly_vert[j]->lonlat[1] += trans_y;
              PhyML_Printf("\n# Sampling == polygon %d vertex @ (%f; %f)",
                           i,
                           poly[i]->poly_vert[j]->lonlat[0],
                           poly[i]->poly_vert[j]->lonlat[1]);
            }
        }
      
      For(i,n_otu) ldsk_a_samp[i] = NULL;

      sample_size = 0;
      For(i,pop_size)
        {
          j = Rand_Int(0,n_poly-1);
          
          if(Is_In_Polygon(ldsk_a_pop[i]->coord,poly[j]) == YES)
            {
              ldsk_a_samp[sample_size] = ldsk_a_pop[i];
              PhyML_Printf("\n@ Coord: %f %f",ldsk_a_samp[sample_size]->coord->lonlat[0],ldsk_a_samp[sample_size]->coord->lonlat[1]);
              sample_size++;
              if(sample_size == n_otu) break;        
            }
        }
      
      area = Area_Of_Poly_Monte_Carlo(poly,n_poly,mmod->lim);
      
      For(j,n_poly) Free_Poly(poly[j]);
      Free(poly);

      if(i == pop_size)
        {
          PhyML_Printf("\n== Not enough individuals in polygon(s) (only %d found).",sample_size);
          /* Generic_Exit(__FILE__,__LINE__,__FUNCTION__);       */
        }
      else break;    
    }
  while(1);
      
  For(i,n_otu) ldsk_a_tips[i] = ldsk_a_samp[i];
  
  tree->disk             = disk;
  disk->ldsk_a           = ldsk_a_tips;
  disk->mmod             = tree->mmod;
  disk->centr->lonlat[0] = .5*tree->mmod->lim->lonlat[0];
  disk->centr->lonlat[1] = .5*tree->mmod->lim->lonlat[1];      
  disk->n_ldsk_a         = n_otu;  

  n_lineages = n_otu;
  do
    {      
      /* printf("\n [%s %f]",disk->id,disk->time); */

      n_remain = 0;
      For(i,n_lineages) 
        {
          /* printf(" %s",ldsk_a_samp[i]->coord->id); */

          if((disk->prev->ldsk != NULL) && (disk->prev->ldsk == ldsk_a_samp[i]->prev)) /* Coalescent event is sampled */
            {
              /* printf("*"); */
              MIGREP_Make_Lindisk_Next(disk->prev->ldsk);
              disk->prev->ldsk->next[disk->prev->ldsk->n_next-1] = ldsk_a_samp[i];
            }
          else
            {
              ldsk_a_tmp[n_remain] = ldsk_a_samp[i];
              n_remain++;
            }
        }

      For(i,n_remain) ldsk_a_samp[i] = ldsk_a_tmp[i];
      if((disk->prev->ldsk != NULL) && (disk->prev->ldsk->n_next > 0)) ldsk_a_samp[i] = disk->prev->ldsk;

      if((disk->prev->ldsk != NULL) && (disk->prev->ldsk->n_next > 0))
        {
          n_lineages -= (disk->prev->ldsk->n_next);
          n_lineages += 1;
        }

      if(n_lineages != n_remain+(disk->prev->ldsk && disk->prev->ldsk->n_next>0)?1:0) 
        {
          PhyML_Printf("\n. n_lineages: %d n_remain: %d n_next: %d",
                       n_lineages,
                       n_remain,
                       disk->prev->ldsk->n_next);
          Generic_Exit(__FILE__,__LINE__,__FUNCTION__);
        }

      /* None of the sampled lineages was hit */
      if((disk->prev->ldsk != NULL) && (disk->prev->ldsk->n_next == 0))
        {
          /* printf("\n. free %s @ %f",disk->ldsk->coord->id,disk->time); */
          Free_Ldisk(disk->prev->ldsk);
          disk->prev->ldsk = NULL;
        }

      disk = disk->prev;
      
      if(disk->prev == NULL)
        {
          PhyML_Printf("\n. Sample has not coalesced completely.");
          Exit("\n");
        }
    }
  while(n_lineages > 1);

  /* For(i,n_otu) printf("\n> %s",tree->disk->ldsk_a[i]->coord->id); */

  disk->prev = NULL;

  disk = tree->disk;
  tree_height = disk->time;
  while(disk)
    {
      disk->time -= tree_height;
      disk = disk->prev;
    }

  Free(ldsk_a_tmp);
  Free(ldsk_a_samp);
  Free(ldsk_a_pop);
  Free(parent_prob);
  
  /* MIGREP_Print_Struct('#',tree); */

  return(area);
 
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
/* Test whether coord is in disk. Will actually only works in disk */
/* is a rectangle... */

int MIGREP_Is_In_Disk(t_geo_coord *coord, t_dsk *disk, t_migrep_mod *mmod)
{
  int i;

  if(!disk->centr->dim) Generic_Exit(__FILE__,__LINE__,__FUNCTION__);

  if(mmod->name == MIGREP_UNIFORM)
    {
      For(i,disk->centr->dim)
        {
          if(FABS(coord->lonlat[i] - disk->centr->lonlat[i]) > disk->mmod->rad + 1.E-20)
            {
              return(NO);
            }
        }
      return(YES);
    }
  else if(mmod->name == MIGREP_NORMAL)
    {
      return(YES);
    }

  return(-1);
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

phydbl MIGREP_Lk(t_tree *tree)
{
  phydbl lnL;
  phydbl log_mu,log_one_mu,log_dens_coal;
  int i,j,n_inter,err;
  short int was_hit, n_hit;
  t_migrep_mod *mmod;
  t_dsk *disk;

  mmod = tree->mmod;
  disk = tree->disk;

  if(disk->next)  Generic_Exit(__FILE__,__LINE__,__FUNCTION__);
  if(!disk->prev) Generic_Exit(__FILE__,__LINE__,__FUNCTION__);    

  log_mu     = LOG(mmod->mu);
  log_one_mu = LOG(1. - mmod->mu);  
  lnL        = 0.0;

  MIGREP_Update_Lindisk_List(tree);

  /* PhyML_Printf("\n\n. New likelihood call"); */
  do
    {
      /* Likelihood for the disk center */
      For(i,disk->mmod->n_dim) lnL -= LOG(disk->mmod->lim->lonlat[i]);
      
      /* PhyML_Printf("\n. Likelihood - disk %s has %d lindisk nodes [%f] rad: %f",disk->id,disk->n_ldsk_a,lnL,mmod->rad); */
      /* fflush(NULL); */
      
      was_hit = NO;
      n_hit   = 0;
      
      For(i,disk->n_ldsk_a)
        {
          was_hit = disk->ldsk_a[i]->prev->disk == disk->prev;
          
          if(MIGREP_Is_In_Ldscape(disk->ldsk_a[i],tree->mmod) == NO)
            {
              mmod->c_lnL = UNLIKELY;
              return(UNLIKELY);
            }

          if(was_hit == YES) n_hit++;
          
          if(was_hit == YES && !disk->prev->ldsk)
            {
              PhyML_Printf("\n== disk: %s disk->prev: %s",disk->id,disk->prev->id);
              Generic_Exit(__FILE__,__LINE__,__FUNCTION__);
            }
          
          switch(mmod->name)
            {
            case MIGREP_UNIFORM:
              {
                if(MIGREP_Is_In_Disk(disk->ldsk_a[i]->coord,disk->prev,mmod) == YES) /* 'Departure' point is in disk */
                  {
                    if(was_hit == YES) /* was hit */
                      {
                        if(MIGREP_Is_In_Disk(disk->prev->ldsk->coord,disk->prev,mmod) == YES) /* 'Arrival' point is in disk */
                          {
                            lnL += log_mu;
                          }
                        else /* Landed outside the disk */
                          {
                            mmod->c_lnL = UNLIKELY;
                            return UNLIKELY;
                          }
                      }
                    else /* was not hit */
                      {
                        lnL += log_one_mu;
                      }
                  }
                else
                  {
                    if(was_hit == YES)
                      {
                        mmod->c_lnL = UNLIKELY;
                        return UNLIKELY;
                      }
                  }                                
                break;
              }
              
            case MIGREP_NORMAL:
              {
                phydbl log_prob_hit;
                
                log_prob_hit = log_mu;
                For(j,mmod->n_dim) log_prob_hit += -POW(disk->ldsk_a[i]->coord->lonlat[j] - disk->prev->centr->lonlat[j],2)/(2.*POW(mmod->rad,2));
                
                if(was_hit == YES)
                  {
                    lnL += log_prob_hit;
                  }
                else
                  {
                    lnL += LOG(1. - EXP(log_prob_hit));
                  }
                break;
              }
              
            default:
              {
                Generic_Exit(__FILE__,__LINE__,__FUNCTION__);
              }
            }
        }

      /* a hit occurred */
      if(n_hit >= 1) 
        {
          switch(mmod->name)
            {
            case MIGREP_UNIFORM: 
              {
                log_dens_coal = MIGREP_Log_Dunif_Rectangle_Overlap(disk->prev->ldsk,disk->prev,mmod);
                lnL += log_dens_coal;
                break;
              }
            case MIGREP_NORMAL:
              {
                err = NO;
                log_dens_coal = 0.0;
                For(j,mmod->n_dim) log_dens_coal += Log_Dnorm_Trunc(disk->prev->ldsk->coord->lonlat[j],
                                                                    disk->prev->centr->lonlat[j],
                                                                    mmod->rad,
                                                                    0.0,
                                                                    mmod->lim->lonlat[j],&err);                    
                lnL += log_dens_coal;
                break;
              }
            }
        }


      disk = disk->prev;
      
      if(!disk->prev) break;
    }
  while(1);
  
  n_inter = MIGREP_Total_Number_Of_Intervals(tree);  
  lnL += (n_inter)*LOG(mmod->lbda) + mmod->lbda*disk->time;
  /* lnL += LnGamma((phydbl)(n_inter)); */
  /* lnL += Dpois((phydbl)n_inter,-mmod->lbda*disk->time,YES); */
  
  if(isinf(lnL) || isnan(lnL)) lnL = UNLIKELY;
  
  mmod->c_lnL = lnL;
  
  return(lnL);
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

phydbl *MIGREP_MCMC(t_tree *tree)
{
  t_mcmc *mcmc;
  int move,i,n_vars;
  phydbl u;
  t_dsk *disk;
  FILE *fp_tree,*fp_stats;
  char *s;
  phydbl *res;
  phydbl true_root_x, true_root_y;
  phydbl max_x, max_y;

  s = (char *)mCalloc(T_MAX_FILE,sizeof(char));

  strcpy(s,"migrep_trees");
  sprintf(s+strlen(s),".%d",tree->mod->io->r_seed);
  fp_tree = Openfile(s,WRITE);
  strcpy(s,"migrep_stats");
  sprintf(s+strlen(s),".%d",tree->mod->io->r_seed);
  fp_stats = Openfile(s,WRITE);
  /* fp_stats = stderr; */

  Free(s);

  mcmc = MCMC_Make_MCMC_Struct();

  disk = tree->disk;
  while(disk->prev) disk = disk->prev;

  tree->mcmc = mcmc;

  mcmc->io               = NULL;
  mcmc->is               = NO;
  mcmc->use_data         = YES;
  mcmc->run              = 0;
  mcmc->chain_len_burnin = 1E+5;
  mcmc->randomize        = YES;
  mcmc->norm_freq        = 1E+3;
  mcmc->max_tune         = 1.E+20;
  mcmc->min_tune         = 1.E-10;
  mcmc->print_every      = 2;
  mcmc->is_burnin        = NO;
  mcmc->nd_t_digits      = 1;
  mcmc->chain_len        = 1E+8;
  mcmc->sample_interval  = 1E+3;  
  mcmc->max_lag          = 1000;
  mcmc->sample_size      = mcmc->chain_len/mcmc->sample_interval;
  mcmc->sample_num       = 0;

  MCMC_Complete_MCMC(mcmc,tree);

  n_vars                 = 12;
  true_root_x            = disk->ldsk->coord->lonlat[0];
  true_root_y            = disk->ldsk->coord->lonlat[1];

  res = (phydbl *)mCalloc(tree->mcmc->chain_len / tree->mcmc->sample_interval * n_vars,sizeof(phydbl));


  MIGREP_Lk(tree);
  Lk(NULL,tree);
  MIGREP_LnPrior_Radius(tree);
  MIGREP_LnPrior_Mu(tree);
  MIGREP_LnPrior_Lbda(tree);

  PhyML_Fprintf(fp_stats,"\n# before rand glnL: %f alnL: %f",tree->mmod->c_lnL,tree->c_lnL);
  PhyML_Fprintf(fp_stats,"\n# ninter: %d",MIGREP_Total_Number_Of_Intervals(tree));
  PhyML_Fprintf(fp_stats,"\n# ncoal: %d",MIGREP_Total_Number_Of_Coal_Disks(tree));
  PhyML_Fprintf(fp_stats,"\n# nhits: %d",MIGREP_Total_Number_Of_Hit_Disks(tree));
  PhyML_Fprintf(fp_stats,"\n# root pos: %f %f",true_root_x,true_root_y);
  PhyML_Fprintf(fp_stats,"\n# root time: %f",disk->time);
  PhyML_Fprintf(fp_stats,"\n# true lbda: %f",tree->mmod->lbda);
  PhyML_Fprintf(fp_stats,"\n# true mu: %f",tree->mmod->mu);
  PhyML_Fprintf(fp_stats,"\n# true rad: %f",tree->mmod->rad);
  PhyML_Fprintf(fp_stats,"\n# true dist parent-offspring: %f",MIGREP_Dist_Parent_To_Offspring(tree));
  PhyML_Fprintf(fp_stats,"\n# true neighborhood size: %f",MIGREP_Neigborhood_Size(tree));
  PhyML_Fprintf(fp_stats,"\n# Fst-based estimate of neighborhood size: %f",MIGREP_Neigborhood_Size_Regression(tree));

  /* s = Write_Tree(tree,NO); */
  /* PhyML_Fprintf(fp_tree,"\n%s",s); */
  /* Free(s); */
  

  /* tree->mmod->lbda = Uni()*(tree->mmod->max_lbda - tree->mmod->min_lbda) + tree->mmod->min_lbda; */
  /* tree->mmod->mu   = Uni()*(tree->mmod->max_mu - tree->mmod->min_mu) + tree->mmod->min_mu; */
  /* tree->mmod->rad  = Uni()*(tree->mmod->max_rad - tree->mmod->min_rad) + tree->mmod->min_rad; */

  tree->mmod->lbda            = Uni()*(0.30 - 0.05) + 0.05;
  tree->mmod->mu              = Uni()*(1.00 - 0.30) + 0.30;
  tree->mmod->rad             = Uni()*(3.00 - 1.00) + 1.00;

  /* disk = tree->disk; */
  /* max_x = 0.0; */
  /* For(i,disk->n_ldsk_a) if(disk->ldsk_a[i]->coord->lonlat[0] > max_x) max_x = disk->ldsk_a[i]->coord->lonlat[0]; */
  /* max_y = 0.0; */
  /* For(i,disk->n_ldsk_a) if(disk->ldsk_a[i]->coord->lonlat[1] > max_y) max_y = disk->ldsk_a[i]->coord->lonlat[1]; */

  /* printf("\n. max_x = %f max_y = %f",max_x,max_y); fflush(stdout); */

  /* tree->mmod->lim->lonlat[0]  = Uni()*(5.*max_x - max_x) + max_x; */
  /* tree->mmod->lim->lonlat[1]  = Uni()*(5.*max_y - max_y) + max_y; */

  /* Random genealogy */
  MIGREP_Simulate_Backward_Core(NO,tree);

  MIGREP_Lk(tree);
  Lk(NULL,tree);
  disk = tree->disk;
  while(disk->prev) disk = disk->prev;

  printf("\n. lk: %f",tree->mmod->c_lnL); fflush(stdout);

  PhyML_Fprintf(fp_stats,"\n# after rand glnL: %f alnL: %f",tree->mmod->c_lnL,tree->c_lnL);
  PhyML_Fprintf(fp_stats,"\n# ninter: %d",MIGREP_Total_Number_Of_Intervals(tree));
  PhyML_Fprintf(fp_stats,"\n# ncoal: %d",MIGREP_Total_Number_Of_Coal_Disks(tree));
  PhyML_Fprintf(fp_stats,"\n# nhits: %d",MIGREP_Total_Number_Of_Hit_Disks(tree));
  PhyML_Fprintf(fp_stats,"\n# root pos: %f %f",true_root_x,true_root_y);
  PhyML_Fprintf(fp_stats,"\n# root time: %f",disk->time);
  PhyML_Fprintf(fp_stats,"\n# start lbda: %f",tree->mmod->lbda);
  PhyML_Fprintf(fp_stats,"\n# start mu: %f",tree->mmod->mu);
  PhyML_Fprintf(fp_stats,"\n# start rad: %f",tree->mmod->rad);

  PhyML_Fprintf(fp_stats,"\n%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s",
                "run",
                "alnL",
                "glnL",
                "lbda",
                "mu",
                "rad",
                "dist",
                "neigh",
                "nInt",
                "nCoal",
                "nHit",
                "xRootLon",
                "xRootLat",
                "rootTime",
                "essLbda",
                "essMu",
                "essRad");


  mcmc->always_yes = NO;
  do
    {
      if(mcmc->run == 1E+6) 
        {
          For(i,mcmc->n_moves) 
            {
              tree->mcmc->adjust_tuning[i] = NO;
            }
        }
      if(mcmc->run == 0) 
        {
          For(i,mcmc->n_moves) 
            {
              tree->mcmc->start_ess[i] = YES;
            }
        }

      u = Uni();

      For(move,tree->mcmc->n_moves) if(tree->mcmc->move_weight[move] > u-1.E-10) break;
      
      if(move == tree->mcmc->n_moves) Generic_Exit(__FILE__,__LINE__,__FUNCTION__);
      
      if(!strcmp(tree->mcmc->move_name[move],"migrep_lbda"))
        MCMC_MIGREP_Lbda(tree);

      if(!strcmp(tree->mcmc->move_name[move],"migrep_mu"))
        MCMC_MIGREP_Mu(tree);

      if(!strcmp(tree->mcmc->move_name[move],"migrep_rad"))
        MCMC_MIGREP_Radius(tree);

      if(!strcmp(tree->mcmc->move_name[move],"migrep_delete_disk"))
        MCMC_MIGREP_Delete_Disk(tree);

      if(!strcmp(tree->mcmc->move_name[move],"migrep_insert_disk"))
        MCMC_MIGREP_Insert_Disk(tree);

      if(!strcmp(tree->mcmc->move_name[move],"migrep_move_disk_ct"))
        MCMC_MIGREP_Move_Disk_Centre(tree);

      if(!strcmp(tree->mcmc->move_name[move],"migrep_move_disk_ud"))
        MCMC_MIGREP_Move_Disk_Updown(tree);

      if(!strcmp(tree->mcmc->move_name[move],"migrep_swap_disk"))
        MCMC_MIGREP_Swap_Disk(tree);

      if(!strcmp(tree->mcmc->move_name[move],"migrep_delete_hit"))
        MCMC_MIGREP_Delete_Hit(tree);

      if(!strcmp(tree->mcmc->move_name[move],"migrep_insert_hit"))
        MCMC_MIGREP_Insert_Hit(tree);

      if(!strcmp(tree->mcmc->move_name[move],"migrep_move_ldsk"))
        MCMC_MIGREP_Move_Ldsk(tree);

      if(!strcmp(tree->mcmc->move_name[move],"migrep_spr"))
        MCMC_MIGREP_Prune_Regraft(tree);

      if(!strcmp(tree->mcmc->move_name[move],"migrep_scale_times"))
        MCMC_MIGREP_Scale_Times(tree);

      /* if(!strcmp(tree->mcmc->move_name[move],"migrep_ldscape_lim")) */
      /*   MCMC_MIGREP_Ldscape_Limits(tree); */

      tree->mcmc->run++;
      MCMC_Get_Acc_Rates(tree->mcmc);
      
      if(!(tree->mcmc->run%tree->mcmc->sample_interval))
        {
          Lk(NULL,tree);
          PhyML_Fprintf(fp_stats,"\n%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%d\t%d\t%d\t%f\t%f\t%f\t%f\t%f\t%f",
                        tree->mcmc->run,
                        tree->c_lnL,
                        tree->mmod->c_lnL,
                        tree->mmod->lbda,
                        tree->mmod->mu,
                        tree->mmod->rad,
                        MIGREP_Dist_Parent_To_Offspring(tree),
                        MIGREP_Neigborhood_Size(tree),
                        MIGREP_Total_Number_Of_Intervals(tree),
                        MIGREP_Total_Number_Of_Coal_Disks(tree),
                        MIGREP_Total_Number_Of_Hit_Disks(tree),
                        true_root_x - disk->ldsk->coord->lonlat[0],
                        true_root_y - disk->ldsk->coord->lonlat[1],
                        disk->time,
                        tree->mcmc->ess[tree->mcmc->num_move_migrep_lbda],
                        tree->mcmc->ess[tree->mcmc->num_move_migrep_mu],
                        tree->mcmc->ess[tree->mcmc->num_move_migrep_rad]);
          fflush(fp_stats);
          
          res[0 * tree->mcmc->chain_len / tree->mcmc->sample_interval +  tree->mcmc->run / tree->mcmc->sample_interval] = tree->mmod->lbda; 
          res[1 * tree->mcmc->chain_len / tree->mcmc->sample_interval +  tree->mcmc->run / tree->mcmc->sample_interval] = tree->mmod->mu; 
          res[2 * tree->mcmc->chain_len / tree->mcmc->sample_interval +  tree->mcmc->run / tree->mcmc->sample_interval] = tree->mmod->rad; 
          res[3 * tree->mcmc->chain_len / tree->mcmc->sample_interval +  tree->mcmc->run / tree->mcmc->sample_interval] = disk->ldsk->coord->lonlat[0];
          res[4 * tree->mcmc->chain_len / tree->mcmc->sample_interval +  tree->mcmc->run / tree->mcmc->sample_interval] = disk->ldsk->coord->lonlat[1];
          res[5 * tree->mcmc->chain_len / tree->mcmc->sample_interval +  tree->mcmc->run / tree->mcmc->sample_interval] = MIGREP_Dist_Parent_To_Offspring(tree);
          res[6 * tree->mcmc->chain_len / tree->mcmc->sample_interval +  tree->mcmc->run / tree->mcmc->sample_interval] = MIGREP_Neigborhood_Size(tree);

          MCMC_Copy_To_New_Param_Val(tree->mcmc,tree);
          
          For(i,tree->mcmc->n_moves)
            {
              if(tree->mcmc->start_ess[i] == YES) MCMC_Update_Effective_Sample_Size(i,tree->mcmc,tree);
            }

          tree->mcmc->sample_num++;
        }

      /* if(!(tree->mcmc->run%(20*tree->mcmc->sample_interval))) */
      /*   { */
          /* char *s = Write_Tree(tree,NO); */
          /* PhyML_Fprintf(fp_tree,"\n%s",s); */
          /* Free(s); */
        /* } */

      if(tree->mcmc->ess[tree->mcmc->num_move_migrep_lbda] > 100. &&
         tree->mcmc->ess[tree->mcmc->num_move_migrep_mu]   > 100. &&
         tree->mcmc->ess[tree->mcmc->num_move_migrep_rad]  > 100.) break;
    }
  while(tree->mcmc->run < tree->mcmc->chain_len);

  fclose(fp_tree);
  fclose(fp_stats);

  return(res);
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

phydbl MIGREP_Wrap_Lk(t_edge *b, t_tree *tree, supert_tree *stree)
{
  return MIGREP_Lk(tree);
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

void MIGREP_New_Traj(t_dsk *start, t_dsk *end, t_tree *tree)
{
  t_dsk *disk;
  int i,j;
  int n_hit_up,n_hit_tot;
  /* phydbl min_up, min_do; */
  /* phydbl max_up, max_do; */



  For(i,start->n_ldsk_a)
    {      
      /* printf("\n<><><>"); */

      disk = end;
      while(disk != start)
        {
          /* disk->ldsk_a[i]->min_coord = GEO_Make_Geo_Coord(disk->mmod->n_dim); */
          /* disk->ldsk_a[i]->max_coord = GEO_Make_Geo_Coord(disk->mmod->n_dim);                     */
          disk = disk->prev;
        }

      disk = end;
      n_hit_tot = 0;
      while(disk != start)
        {
          if(disk->ldsk_a[i]->is_hit == YES) n_hit_tot++;
          disk = disk->prev;
        }


      /* printf("\n. n_hit_tot: %d",n_hit_tot); */
      /* fflush(NULL); */

      /* printf("\n. Start disk name: %s lindisk %s at %.2f [%.2f %.2f] end disk: %s lindisk %s at %.2f [%.2f %.2f]", */
      /*        start->id, */
      /*        start->ldsk_a[i]->coord->id, */
      /*        start->time, */
      /*        start->ldsk_a[i]->coord->lonlat[0], */
      /*        start->ldsk_a[i]->coord->lonlat[1], */
      /*        end->id, */
      /*        end->ldsk_a[i]->coord->id, */
      /*        end->time, */
      /*        end->ldsk_a[i]->coord->lonlat[0], */
      /*        end->ldsk_a[i]->coord->lonlat[1] */
      /*        ); */
      /* disk = end; */
      /* while(disk != start) */
      /*   { */
      /*     printf("\n. %d %s %.2f %.2f", */
      /*            i, */
      /*            disk->ldsk_a[i]->coord->id, */
      /*            disk->ldsk_a[i]->coord->lonlat[0], */
      /*            disk->ldsk_a[i]->coord->lonlat[1]); */
      /*     disk = disk->prev; */
      /*   } */
      
      n_hit_up = 0;
      disk = end;
      while(disk->prev != start)
        {
          if(disk->ldsk_a[i]->is_hit == YES)
            {              
              n_hit_up++;

              For(j,disk->mmod->n_dim)
                {
                  /* min_up = end->ldsk_a[i]->coord->lonlat[j] - n_hit_up * 2. * disk->mmod->rad; */
                  /* min_do = start->ldsk_a[i]->coord->lonlat[j] - (n_hit_tot - n_hit_up) * 2. * disk->mmod->rad; */
                  /* disk->ldsk_a[i]->prev->min_coord->lonlat[j] =  */
                  /*   MAX(MAX(MAX(min_up,min_do),0.0),disk->prev->centr->lonlat[j] - disk->mmod->rad); */

                  /* max_up = end->ldsk_a[i]->coord->lonlat[j] + n_hit_up * 2. * disk->mmod->rad; */
                  /* max_do = start->ldsk_a[i]->coord->lonlat[j] + (n_hit_tot - n_hit_up) * 2. * disk->mmod->rad; */
                  /* disk->ldsk_a[i]->prev->max_coord->lonlat[j] =  */
                  /*   MIN(MIN(MIN(max_up,max_do),disk->mmod->lim->lonlat[j]),disk->prev->centr->lonlat[j] + disk->mmod->rad); */

                  /* printf("\n. curr: %s min_up: %.2f min_do: %.2f max_up: %.2f max_do: %.2f %d %d start: %.2f [%.2f %.2f]", */
                  /*        disk->ldsk_a[i]->prev->coord->id, */
                  /*        min_up,min_do,max_up,max_do, */
                  /*        n_hit_tot,n_hit_up, */
                  /*        start->ldsk_a[i]->coord->lonlat[j], */
                  /*        disk->ldsk_a[i]->prev->min_coord->lonlat[j], */
                  /*        disk->ldsk_a[i]->prev->max_coord->lonlat[j]); */
                }
            }
          disk = disk->prev;
        }
      
      disk = end;
      while(disk->prev != start)
        {
          if(disk->ldsk_a[i]->is_hit == YES)
            {
              /* For(j,disk->mmod->n_dim) */
                /* disk->ldsk_a[i]->prev->coord->lonlat[j] =  */
                /* Uni()* */
                /* (disk->ldsk_a[i]->prev->max_coord->lonlat[j]  - */
                /*  disk->ldsk_a[i]->prev->min_coord->lonlat[j]) + */
                /* disk->ldsk_a[i]->prev->min_coord->lonlat[j]; */
            }
          else
            {
              For(j,disk->mmod->n_dim)
                disk->ldsk_a[i]->prev->coord->lonlat[j] = 
                disk->ldsk_a[i]->coord->lonlat[j];
            }

          disk = disk->prev;
        }
      
      disk = end;
      while(disk != start)
        {
          /* Free_Geo_Coord(disk->ldsk_a[i]->min_coord); */
          /* Free_Geo_Coord(disk->ldsk_a[i]->max_coord); */
          disk = disk->prev;
        }
    }

  /* gtk_widget_queue_draw(tree->draw_area); */
  
  /* For(i,start->n_ldsk_a) */
  /*   { */
  /*     printf("\n<><><>"); */
  /*     disk = end; */
  /*     while(disk != start) */
  /*       { */
  /*         printf("\nx %s %.2f %.2f centr: %.2f %.2f", */
  /*                disk->ldsk_a[i]->coord->id, */
  /*                disk->ldsk_a[i]->coord->lonlat[0], */
  /*                disk->ldsk_a[i]->coord->lonlat[1], */
  /*                disk->centr->lonlat[0], */
  /*                disk->centr->lonlat[1]); */
  /*         disk = disk->prev; */
  /*       } */
  /*     fflush(NULL); */
  /*   } */
  
  /* MIGREP_Lk(tree); */
  /* printf("\n. >> Lk: %f rad: %f",tree->mmod->c_lnL,tree->mmod->rad); */
  /* sleep(5); */
}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

void MIGREP_Remove_Disk(t_dsk *disk)
{
  t_dsk *prev;
  t_dsk *next;

  prev = disk->prev;
  next = disk->next;

  /* printf("\n. Remove disk %s (prev: %s next: %s)", */
  /*        disk->id, */
  /*        prev?prev->id:NULL, */
  /*        next?next->id:NULL); fflush(NULL); */

  if(prev == NULL) Generic_Exit(__FILE__,__LINE__,__FUNCTION__);
  if(next == NULL) Generic_Exit(__FILE__,__LINE__,__FUNCTION__);    
  
  prev->next = next;
  next->prev = prev;

  /* printf(" set %s->next to %s and %s->prev to %s", */
  /*        prev->id,prev->next->id, */
  /*        next->id,next->prev->id); fflush(NULL); */
}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

/* Insert disk event. disk->prev and disk->next need to be set 
   accordingly. 
*/
void MIGREP_Insert_Disk(t_dsk *disk)
{

  if(disk->prev == NULL) Generic_Exit(__FILE__,__LINE__,__FUNCTION__);    
  if(disk->next == NULL) Generic_Exit(__FILE__,__LINE__,__FUNCTION__);    
  if(disk->prev->time > disk->time) Generic_Exit(__FILE__,__LINE__,__FUNCTION__);
  if(disk->next->time < disk->time) Generic_Exit(__FILE__,__LINE__,__FUNCTION__);

  /* printf("\n. Insert disk %s @ %f between %s and %s", */
  /*        disk->id,disk->time, */
  /*        disk->prev->id, */
  /*        disk->next->id); */
  /* fflush(NULL); */

  disk->prev->next = disk;
  disk->next->prev = disk;
}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

t_ldsk *MIGREP_Prev_Coal_Lindisk(t_ldsk *t)
{
  if(t == NULL) return NULL;

  if(t->n_next > 1) 
    {
      return t;
    }
  else
    {
      return MIGREP_Prev_Coal_Lindisk(t->prev);
    }
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

t_ldsk *MIGREP_Next_Coal_Lindisk(t_ldsk *t)
{
  if(t == NULL) Generic_Exit(__FILE__,__LINE__,__FUNCTION__);    

  if(t->n_next > 1 || t->next == NULL) return t;
  else
    {
      if(t->n_next > 1) // Should have t->is_coal = YES
        {
          PhyML_Printf("\n== Err. in file %s at line %d (function '%s') \n",__FILE__,__LINE__,__FUNCTION__);
          Warn_And_Exit("");
        }
      return MIGREP_Next_Coal_Lindisk(t->next[0]);
    }
}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

/*  Generate a new trajectory, including disk event centers, between  
    'y_ldsk' a ``young'' lindisk event and 'o_ldsk' an old one. 'y_ldsk 
    and 'o_ldsk' remain unaffected. No disk events should be present    
    between y_ldsk and o_ldsk: we need to generate some first. n_cur_disk
    is the current number of disks between y_ldsk and o_ldsk.
*/
int MIGREP_One_New_Traj(t_ldsk *y_ldsk, t_ldsk *o_ldsk, int dir_o_y, t_dsk *xtra_dsk, int n_cur_disk, t_tree *tree)
{
  t_migrep_mod *mmod;
  t_dsk *disk,**disk_new;
  int i,n,K;
  int min_n_disk,n_new_disk,n_disk;

  mmod     = tree->mmod;
  disk     = NULL;
  disk_new = NULL;
  K        = 2;

  /* printf("\n# New traj from %s to %s",y_ldsk->coord->id,o_ldsk->coord->id); */
  /* fflush(NULL); */

  /* Minimum number of disks between y_ldsk and o_ldsk */
  min_n_disk = 0;
  For(i,mmod->n_dim)
    {
      /* PhyML_Printf("\n# y_ldsk %s : %f o_ldsk->disk->centr: %f rad: %f", */
      /*              y_ldsk->coord->id, */
      /*              y_ldsk->coord->lonlat[i], */
      /*              o_ldsk->disk->centr->lonlat[i], */
      /*              mmod->rad); */
      if(y_ldsk->coord->lonlat[i] < o_ldsk->disk->centr->lonlat[i])
        {
          n_disk = 0;
          while(y_ldsk->coord->lonlat[i] + (2*n_disk-1)*mmod->rad < o_ldsk->disk->centr->lonlat[i] - mmod->rad) n_disk++;
          if(n_disk > min_n_disk) min_n_disk = n_disk;
        }
      else
        {
          n_disk = 0;
          while(y_ldsk->coord->lonlat[i] - (2*n_disk-1)*mmod->rad > o_ldsk->disk->centr->lonlat[i] + mmod->rad) n_disk++;
          if(n_disk > min_n_disk) min_n_disk = n_disk;
        }
      /* printf("  -- min_n_disk: %d",min_n_disk); */
    }
  
  /* printf("\n# min_n_disk: %d cur_n_disk: %d",min_n_disk,n_cur_disk); */
  /* fflush(NULL); */
  
  /* How many disks along the new path between y_ldsk and o_ldsk */
  n_new_disk = Rand_Int(n_cur_disk-K,n_cur_disk+K);
  if(n_new_disk < min_n_disk) n_new_disk = min_n_disk;

  if(xtra_dsk != NULL) n_new_disk++;

  /* printf("\n# Add n_new_disk: %d",n_new_disk); fflush(NULL); */
  
  if(n_new_disk > 0)
    {
      /* Make new disks to create a new path between ldsk_left and ldsk_up */
      disk_new = (t_dsk **)mCalloc(n_new_disk,sizeof(t_dsk *));
      For(i,n_new_disk-1)  disk_new[i] = MIGREP_Make_Disk_Event(mmod->n_dim,tree->n_otu);
      if(xtra_dsk != NULL) disk_new[n_new_disk-1] = xtra_dsk;
      else                 disk_new[n_new_disk-1] = MIGREP_Make_Disk_Event(mmod->n_dim,tree->n_otu);

      For(i,n_new_disk)  MIGREP_Init_Disk_Event(disk_new[i],mmod->n_dim,mmod);

      /* Times of these new disks. If xtra_dsk != NULL, then make sure you do not */
      /* reset the time of that disk  */
      n = (xtra_dsk != NULL) ? (n_new_disk-1) : (n_new_disk);
      For(i,n)
        disk_new[i]->time =
        Uni()*(y_ldsk->disk->time - o_ldsk->disk->time) + o_ldsk->disk->time;       
     
      /* Insert these events */
      For(i,n_new_disk)
        {
          if(tree->disk->next) Generic_Exit(__FILE__,__LINE__,__FUNCTION__);
          disk = tree->disk;
          while(disk->time > disk_new[i]->time) disk = disk->prev;
          disk_new[i]->prev = disk;
          disk_new[i]->next = disk->next;
          MIGREP_Insert_Disk(disk_new[i]);
        }
            
      /* For(i,n_new_disk) */
      /*   { */
      /*     printf("\n> disk_new: %f [%s]",disk_new[i]->time,disk_new[i]->id); fflush(NULL); */
      /*   } */
      
      /* Add new lindisks to the new disk events */
      For(i,n_new_disk)
        {
          disk_new[i]->ldsk = MIGREP_Make_Lindisk_Node(tree->mmod->n_dim);
          MIGREP_Init_Lindisk_Node(disk_new[i]->ldsk,disk_new[i],tree->mmod->n_dim);
          MIGREP_Make_Lindisk_Next(disk_new[i]->ldsk);
          /* printf("\n# Add ldsk %s to %s",disk_new[i]->ldsk->coord->id,disk_new[i]->id); fflush(NULL); */
        }
      
      /* Connect them */
      MIGREP_Connect_Ldsk_Given_Disk(disk_new,n_new_disk,y_ldsk,o_ldsk,dir_o_y);

      Free(disk_new);
    }
  else
    {
      o_ldsk->next[dir_o_y] = y_ldsk;
      y_ldsk->prev          = o_ldsk;
    }
 
  /* Generate a trajectory */
  MIGREP_One_New_Traj_Given_Disk(y_ldsk,o_ldsk,tree);  

  return(n_new_disk);
}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

/* Generate a new trajectory, including disk event centers, between 
  'y_ldsk' a ``young'' lindisk event and 'o_ldsk' an old one. 'y_ldsk 
  and 'o_ldsk' remain unaffected. Disk events between these two ldsk 
  should already be set. 
*/
void MIGREP_One_New_Traj_Given_Disk(t_ldsk *y_ldsk, t_ldsk *o_ldsk, t_tree *tree)
{
  int n_disk_btw;
  t_ldsk *ldsk;
  phydbl min, max;
  phydbl *min_disk_coord, *max_disk_coord;
  int i,k;
  phydbl rad;


  /* Number of disks between y_ldsk and o_ldsk */
  ldsk = y_ldsk;
  n_disk_btw = 0;
  while(ldsk->prev != o_ldsk)
    {
      n_disk_btw++;
      ldsk = ldsk->prev;
    }

  if(n_disk_btw == 0) return;

  ldsk = y_ldsk;
  rad  = ldsk->disk->mmod->rad;
  k    = 0;
  while(ldsk != o_ldsk)
    {
      if(!ldsk->disk->next) /* Don't change location at tip node */
        {
          ldsk = ldsk->prev;
          continue;
        }
      
      MIGREP_Store_Geo_Coord(ldsk->coord);
      MIGREP_Store_Geo_Coord(ldsk->disk->centr);

      For(i,tree->mmod->n_dim)
        {
          min = 
            MAX(0.0,
                MAX(ldsk->coord->lonlat[i] - 2.*rad,
                    o_ldsk->disk->centr->lonlat[i] - rad*(2.*(n_disk_btw-k)-1.)));

          max = 
            MIN(ldsk->disk->mmod->lim->lonlat[i],
                MIN(ldsk->coord->lonlat[i] + 2.*rad,
                    o_ldsk->disk->centr->lonlat[i] + rad*(2.*(n_disk_btw-k)-1.)));
          
          if(max < min) Generic_Exit(__FILE__,__LINE__,__FUNCTION__);
                
          /* New coordinates for the lindisk */
          ldsk->coord->lonlat[i] = Uni()*(max - min) + min;
        }

      /* New coordinate for the centre of the corresponding disk event */
      MIGREP_Get_Min_Max_Disk_Given_Ldsk(ldsk->disk,&min_disk_coord,&max_disk_coord,tree);
      For(i,tree->mmod->n_dim) ldsk->disk->centr->lonlat[i] = Uni()*(max_disk_coord[i] - min_disk_coord[i]) + min_disk_coord[i];
      Free(min_disk_coord);
      Free(max_disk_coord);

      ldsk = ldsk->prev;
      k++;
    }
}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

phydbl MIGREP_Uniform_Path_Density(t_ldsk *y_ldsk, t_ldsk *o_ldsk, t_tree *tree)
{
  int n_disk_btw;
  t_ldsk *ldsk;
  phydbl min, max;
  int i,k;
  phydbl rad;
  phydbl log_dens;
  phydbl *min_disk_coord, *max_disk_coord;

  if(y_ldsk == o_ldsk) return .0;

  /* Number of disks between y_ldsk and o_ldsk */
  ldsk = y_ldsk;
  n_disk_btw = 0;
  while(ldsk->prev != o_ldsk)
    {
      n_disk_btw++;
      ldsk = ldsk->prev;
    }

  if(n_disk_btw == 0) return .0;

  log_dens = 0.0;
  ldsk     = y_ldsk;
  rad      = ldsk->disk->mmod->rad;
  k        = 0;
  while(ldsk != o_ldsk)
    {
      if(!ldsk->disk->next)
        {
          ldsk = ldsk->prev;
          continue;
        }
      
      For(i,ldsk->disk->mmod->n_dim)
        {
          min = 
            MAX(0.0,
                MAX(ldsk->coord->lonlat[i] - 2.*rad,
                    o_ldsk->disk->centr->lonlat[i] - rad*(2.*(n_disk_btw-k)-1.)));

          max = 
            MIN(ldsk->disk->mmod->lim->lonlat[i],
                MIN(ldsk->coord->lonlat[i] + 2.*rad,
                    o_ldsk->disk->centr->lonlat[i] + rad*(2.*(n_disk_btw-k)-1.)));

          if(max < min) Generic_Exit(__FILE__,__LINE__,__FUNCTION__);

          log_dens -= LOG(max - min);          
        }

      MIGREP_Get_Min_Max_Disk_Given_Ldsk(ldsk->disk,&min_disk_coord,&max_disk_coord,tree);
      For(i,tree->mmod->n_dim) log_dens -= LOG(max_disk_coord[i] - min_disk_coord[i]);
      Free(min_disk_coord);
      Free(max_disk_coord);

      ldsk = ldsk->prev;
      k++;
    }

  return(log_dens);
}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

/* Return the index of the 'next' element of 'old' that should be
   used in order to reach 'young'. 
*/
int MIGREP_Get_Next_Direction(t_ldsk *young, t_ldsk *old)
{
  if(young->disk->time < old->disk->time)
    {
      PhyML_Printf("\n== young (%s) @ time %f; old (%s) @ time %f",
                   young->coord->id,young->disk->time,
                   old->coord->id,old->disk->time);
      Generic_Exit(__FILE__,__LINE__,__FUNCTION__);    
    }
  
  if(young == NULL) Generic_Exit(__FILE__,__LINE__,__FUNCTION__);    
  
  if(young->prev == old)
    {
      int i;
      For(i,old->n_next) if(old->next[i] == young) return i;
    }
  else
    {
      return MIGREP_Get_Next_Direction(young->prev,old);
    }
  return(-1);
}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

/* void MIGREP_Update_Lindisk_List(phydbl time, t_ldsk **list, int *pos, t_dsk *disk) */
/* { */
/*   t_dsk *root_dsk; */

/*   *pos = 0; */
/*   root_dsk = disk; */
/*   while(root_dsk->prev) root_dsk = root_dsk->prev; */
/*   /\* printf("\n. root_dsk: %s",root_dsk?root_dsk->id:"xx"); *\/ */
/*   MIGREP_Update_Lindisk_List_Pre(root_dsk->ldsk,time,list,pos); */
/* } */

/* /\*\//////////////////////////////////////////////////////////// */
/* ////////////////////////////////////////////////////////////\*\/ */

/* void MIGREP_Update_Lindisk_List_Pre(t_ldsk *ldsk, phydbl time, t_ldsk **list, int *pos) */
/* { */
/*   /\* printf("\n. time: %f pos: %d ldsk: %s disk: %s n_next: %d ldsk->disk->time: %f disk: %s", *\/ */
/*   /\*        time, *\/ */
/*   /\*        *pos, *\/ */
/*   /\*        ldsk ? ldsk->coord->id : "xx", *\/ */
/*   /\*        ldsk ? ldsk->disk->id : "zz", *\/ */
/*   /\*        ldsk ? ldsk->n_next : -1, *\/ */
/*   /\*        ldsk ? ldsk->disk->time : -1., *\/ */
/*   /\*        ldsk ? ldsk->disk->id : "yy"); fflush(NULL); *\/ */

/*   if(ldsk == NULL) Generic_Exit(__FILE__,__LINE__,__FUNCTION__); */

/*   if((ldsk->prev != NULL) && (ldsk->disk->time > time) && (ldsk->prev->disk->time < time)) */
/*     { */
/*       list[*pos] = ldsk; */
/*       *pos = *pos + 1; */
/*     } */
/*   else if(Are_Equal(ldsk->disk->time,time,1.E-10)) */
/*     { */
/*       list[*pos] = ldsk; */
/*       *pos = *pos + 1;       */
/*     } */
/*   else if(ldsk->disk->time < time) */
/*     { */
/*       int i; */
/*       For(i,ldsk->n_next) */
/*         MIGREP_Update_Lindisk_List_Pre(ldsk->next[i],time,list,pos);     */
/*     } */
/*   else Generic_Exit(__FILE__,__LINE__,__FUNCTION__); */
    
/* } */

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

void MIGREP_Update_Lindisk_List(t_tree *tree)
{
  MIGREP_Update_Lindisk_List_Pre(tree->disk->prev);
}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

void MIGREP_Update_Lindisk_List_Pre(t_dsk *disk)
{
  if(!disk) return;
  else
    {
      MIGREP_Update_Lindisk_List_Core(disk);
      MIGREP_Update_Lindisk_List_Pre(disk->prev);    
    }
}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

void MIGREP_Update_Lindisk_List_Core(t_dsk *disk)
{
  int i;

  if(!disk->next) return;

  disk->n_ldsk_a = 0;
  For(i,disk->next->n_ldsk_a)
    {
      if(disk->next->ldsk_a[i]->prev != disk->ldsk)
        {
          disk->ldsk_a[disk->n_ldsk_a] = disk->next->ldsk_a[i];
          disk->n_ldsk_a++;
        }
    }
  
  if(disk->ldsk) 
    {
      disk->ldsk_a[disk->n_ldsk_a] = disk->ldsk;
      disk->n_ldsk_a++;          
    }
  
  if(disk->n_ldsk_a == 0) 
    {
      PhyML_Printf("\n== disk %s",disk->id);
      Generic_Exit(__FILE__,__LINE__,__FUNCTION__);
    }
}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

/* Connect all the ldsk between y_ldsk (young ldsk) and o_ldsk (old ldsk).
   The disk between y_ldsk and o_ldsk should have all been set already
   Note: the disks in **disk are sorted in ascending order of their 
   times
*/

void MIGREP_Connect_Ldsk_Given_Disk(t_dsk **disk, int n_disk, t_ldsk *y_ldsk, t_ldsk *o_ldsk, int dir_o_y)
{
  int i,j;
  t_dsk *disk_tmp;

  /* Sort these events by ascending order of their times */
  For(i,n_disk-1)
    {
      for(j=i+1;j<n_disk;j++)
        {
          if(disk[j]->time > disk[i]->time)
            {
              disk_tmp = disk[i];
              disk[i]  = disk[j];
              disk[j]  = disk_tmp;
            }
        }
    }


  For(i,n_disk)
    {
      if(!i)
        {
          disk[i]->ldsk->next[0] = y_ldsk;
          y_ldsk->prev           = disk[i]->ldsk;
          /* printf("\n. connect %s to %s",disk[i]->ldsk->coord->id,y_ldsk->coord->id); fflush(NULL); */
        }
      else
        {
          disk[i]->ldsk->next[0] = disk[i-1]->ldsk;
          disk[i-1]->ldsk->prev  = disk[i]->ldsk;
          /* printf("\n. connect %s to %s",disk[i]->ldsk->coord->id,disk[i-1]->ldsk->coord->id); fflush(NULL); */
        }
    }
  
  /* printf("\n. connect %s next dir: %d [%d] to %s",o_ldsk->coord->id,dir_o_y,o_ldsk->n_next,disk[n_disk-1]->ldsk->coord->id); fflush(NULL); */
  o_ldsk->next[dir_o_y]      = disk[n_disk-1]->ldsk;
  disk[n_disk-1]->ldsk->prev = o_ldsk;

}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

void MIGREP_Print_Struct(char sign, t_tree *tree)
{
  t_dsk *disk;
  int i,j;
  t_ldsk *ldisk;

  disk = tree->disk;
  while(disk->prev) disk = disk->prev;
  do
    {
      printf("\n%c Disk: %s @ %7.3f has %3s on it is_coal? %2d rad: %f coord ",
             sign,
             disk->id,
             disk->time,disk->ldsk?disk->ldsk->coord->id:NULL,
             disk->ldsk?disk->ldsk->n_next:-1,
             tree->mmod->rad); fflush(NULL);
      For(j,tree->mmod->n_dim) PhyML_Printf(" %f",disk->centr->lonlat[j]);

      MIGREP_Update_Lindisk_List(tree);

      For(i,disk->n_ldsk_a)
        {
          ldisk = disk->ldsk_a[i];

          printf("\n%c ldisk: %s prev: %s",
                 sign,
                 ldisk->coord->id,
                 ldisk->prev ? ldisk->prev->coord->id : NULL);

          For(j,tree->mmod->n_dim) 
            {
              PhyML_Printf(" %f",ldisk->coord->lonlat[j]);

              if(FABS(ldisk->coord->lonlat[j] - ldisk->disk->centr->lonlat[j]) > 2.*tree->mmod->rad &&
                 ldisk->disk->ldsk == ldisk) PhyML_Printf(" ! ");

              if(ldisk->prev)
                {
                  if(ldisk->coord->lonlat[j] < ldisk->prev->disk->centr->lonlat[j] - tree->mmod->rad) PhyML_Printf(" #a ");
                  if(ldisk->coord->lonlat[j] > ldisk->prev->disk->centr->lonlat[j] + tree->mmod->rad) PhyML_Printf(" #b ");
                }

              if(ldisk->next)
                {
                  if(ldisk->coord->lonlat[j] < ldisk->disk->centr->lonlat[j] - tree->mmod->rad) PhyML_Printf(" $a ");
                  if(ldisk->coord->lonlat[j] > ldisk->disk->centr->lonlat[j] + tree->mmod->rad) PhyML_Printf(" $b ");
                }
            }
        }

      disk = disk->next;      
    }
  while(disk);
}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

void MIGREP_Check_Struct(t_tree *tree)
{
  t_dsk *disk;
  int i,j;
  t_ldsk *ldisk;

  disk = tree->disk;
  while(disk->prev) disk = disk->prev;
  do
    {
      /* MIGREP_Update_Lindisk_List(disk->time,disk->ldsk_a,&(disk->n_ldsk_a),disk); */
      MIGREP_Update_Lindisk_List(tree);

      For(i,disk->n_ldsk_a)
        {
          ldisk = disk->ldsk_a[i];
          if(ldisk->prev != NULL)
            {
              For(j,tree->mmod->n_dim)
                {
                  if(FABS(ldisk->coord->lonlat[j] - 
                          ldisk->prev->coord->lonlat[j]) > 2.*tree->mmod->rad)
                    {
                      MIGREP_Print_Struct('=',tree);
                      PhyML_Printf("\n== %f %f %f",
                                   ldisk->coord->lonlat[j], 
                                   ldisk->prev->coord->lonlat[j],
                                   2.*tree->mmod->rad);
                      PhyML_Printf("\n== Radius: %f",tree->mmod->rad);
                      PhyML_Printf("\n== Check ldsk %s",ldisk->coord->id);
                      PhyML_Printf("\n== Centr: %f",ldisk->prev->disk->centr->lonlat[j]);
                      Generic_Exit(__FILE__,__LINE__,__FUNCTION__);
                    }
                }
            }
        }

      disk = disk->next;      
    }
  while(disk);
}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

void MIGREP_Store_Geo_Coord(t_geo_coord *t)
{
  int i;

  For(i,t->dim) t->cpy->lonlat[i] = t->lonlat[i];  
  t->cpy->dim = t->dim;
  strcpy(t->cpy->id,t->id);
}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/


void MIGREP_Restore_Geo_Coord(t_geo_coord *t)
{
  int i;

  For(i,t->dim) t->lonlat[i] = t->cpy->lonlat[i];  
  t->dim = t->cpy->dim;
  strcpy(t->id,t->cpy->id);
}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

int MIGREP_Total_Number_Of_Intervals(t_tree *tree)
{
  t_dsk *disk;
  int n_intervals;

  if(tree->disk->next) Generic_Exit(__FILE__,__LINE__,__FUNCTION__);

  disk = tree->disk;
  n_intervals = 0;
  while(disk->prev)
    {
      n_intervals++;
      disk = disk->prev;
    }
  return(n_intervals);

}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

int MIGREP_Total_Number_Of_Hit_Disks(t_tree *tree)
{
  t_dsk *disk;
  int n_hit_disks;

  if(tree->disk->next) Generic_Exit(__FILE__,__LINE__,__FUNCTION__);

  disk = tree->disk;
  n_hit_disks = 0;
  while(disk)
    {
      if(disk->ldsk) n_hit_disks++;
      disk = disk->prev;
    }
  return(n_hit_disks);

}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

int MIGREP_Total_Number_Of_Coal_Disks(t_tree *tree)
{
  t_dsk *disk;
  int n_coal_disks;

  if(tree->disk->next) Generic_Exit(__FILE__,__LINE__,__FUNCTION__);
  disk = tree->disk;
  n_coal_disks = 0;
  while(disk)
    {
      if(disk->ldsk && disk->ldsk->n_next > 1) n_coal_disks++;
      disk = disk->prev;
    }

  return(n_coal_disks);
}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

phydbl MIGREP_Log_Dunif_Rectangle_Overlap(t_ldsk *ldsk, t_dsk *disk, t_migrep_mod *mmod)
{
  phydbl up, down, left, rght;
  phydbl log_dens;
  /* phydbl main_mass,l; */

  log_dens  = 0.0;
  /* main_mass = 1. - mmod->soft_bound_area; */

  up   = MIN(disk->centr->lonlat[0] + mmod->rad, mmod->lim->lonlat[0]);
  down = MAX(disk->centr->lonlat[0] - mmod->rad, 0.0);
  rght = MIN(disk->centr->lonlat[1] + mmod->rad, mmod->lim->lonlat[1]);
  left = MAX(disk->centr->lonlat[1] - mmod->rad, 0.0);


  /* l = main_mass / (.5*(1.-main_mass)*(up - down)); */
  /* if(ldsk->coord->lonlat[0] < down)    log_dens += LOG(.5*(1.-main_mass)) + LOG(l) - l*(down - ldsk->coord->lonlat[0]); */
  /* else if(ldsk->coord->lonlat[0] > up) log_dens += LOG(.5*(1.-main_mass)) + LOG(l) - l*(ldsk->coord->lonlat[0] - up); */
  /* else log_dens += LOG(main_mass) - LOG(up - down); */

  /* l = main_mass / (.5*(1.-main_mass)*(rght - left)); */
  /* if(ldsk->coord->lonlat[1] < left)      log_dens += LOG(.5*(1.-main_mass)) + LOG(l) - l*(left - ldsk->coord->lonlat[1]); */
  /* else if(ldsk->coord->lonlat[1] > rght) log_dens += LOG(.5*(1.-main_mass)) + LOG(l) - l*(ldsk->coord->lonlat[1] - rght); */
  /* else log_dens += LOG(main_mass) - LOG(rght - left); */


  if(ldsk->coord->lonlat[0] < down)  return UNLIKELY;
  if(ldsk->coord->lonlat[0] > up)    return UNLIKELY;
  if(ldsk->coord->lonlat[1] < left)  return UNLIKELY;
  if(ldsk->coord->lonlat[1] > rght)  return UNLIKELY;

  log_dens = -LOG(up-down)-LOG(rght-left);

  return(log_dens);
}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/
/* Samples uniformly within a rectangle (with truncation for border) */
/* and returns the corresponding log density */
phydbl MIGREP_Runif_Rectangle_Overlap(t_ldsk *ldsk, t_dsk *disk, t_migrep_mod *mmod)
{
  phydbl up, down, left, rght;

  up   = MIN(disk->centr->lonlat[0] + mmod->rad, mmod->lim->lonlat[0]);
  down = MAX(disk->centr->lonlat[0] - mmod->rad, 0.0);
  rght = MIN(disk->centr->lonlat[1] + mmod->rad, mmod->lim->lonlat[1]);
  left = MAX(disk->centr->lonlat[1] - mmod->rad, 0.0);

  ldsk->coord->lonlat[0] = Uni()*(up - down) + down;
  ldsk->coord->lonlat[1] = Uni()*(rght - left) + left;

  /* printf("\n. disk %s (%f %f) rad: %f up: %f down: %f rght: %f left: %f", */
  /*        disk->id, */
  /*        disk->centr->lonlat[0], */
  /*        disk->centr->lonlat[1], */
  /*        mmod->rad, */
  /*        up,down,rght,left); */
  return(LOG(up-down)+LOG(rght-left));
}
 
/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/
 
phydbl MIGREP_Rnorm_Trunc(t_ldsk *ldsk, t_dsk *disk, t_migrep_mod *mmod)
{
  phydbl up, down, left, rght;
  int err;

  up   = mmod->lim->lonlat[0];
  down = 0.0;
  rght = mmod->lim->lonlat[1];
  left = 0.0;

  err = NO;
  ldsk->coord->lonlat[0] = Rnorm_Trunc(disk->centr->lonlat[0],mmod->rad,down,up,&err);
  ldsk->coord->lonlat[1] = Rnorm_Trunc(disk->centr->lonlat[1],mmod->rad,left,rght,&err);

  return(0.0);
  
}
 
/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

phydbl MIGREP_Wrap_Prior_Radius(t_edge *e, t_tree *tree, supert_tree *st)
{
  return MIGREP_LnPrior_Radius(tree);
}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

phydbl MIGREP_LnPrior_Lbda(t_tree *tree)
{
  /* tree->mmod->c_ln_prior_lbda = LOG(tree->mmod->prior_param_lbda) - tree->mmod->prior_param_lbda * tree->mmod->lbda; */
  tree->mmod->c_ln_prior_lbda = 1.0;
  return(tree->mmod->c_ln_prior_lbda);
}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

phydbl MIGREP_LnPrior_Mu(t_tree *tree)
{
  /* tree->mmod->c_ln_prior_mu = LOG(tree->mmod->prior_param_mu) - tree->mmod->prior_param_mu * (1. - tree->mmod->mu) - LOG(1.-EXP(-tree->mmod->prior_param_mu)); */
  tree->mmod->c_ln_prior_mu = 1.0;
  return(tree->mmod->c_ln_prior_mu);
}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

phydbl MIGREP_LnPrior_Radius(t_tree *tree)
{
  /* tree->mmod->c_ln_prior_rad = LOG(tree->mmod->prior_param_rad) - tree->mmod->prior_param_rad * tree->mmod->rad; */
  tree->mmod->c_ln_prior_rad = 1.0;
  return(tree->mmod->c_ln_prior_rad);
}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

void MIGREP_Initial_Ldsk_Pos(t_tree *tree)
{
  t_dsk *disk;
  int i,j;
  phydbl mean;

  disk = tree->disk->prev;

  do
    {
      if(disk->ldsk)
        {
          For(i,tree->mmod->n_dim)
            {
              mean = 0.0;
              For(j,disk->ldsk->n_next) mean += disk->ldsk->next[j]->coord->lonlat[i];
              disk->ldsk->coord->lonlat[i] = mean / (phydbl)disk->ldsk->n_next;
            }
        }
      disk = disk->prev;
    }
  while(disk);


}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

phydbl MIGREP_Min_Radius(t_tree *tree)
{
  phydbl ori_rad, min_rad;

  ori_rad = tree->mmod->rad;
  tree->mmod->rad = tree->mmod->max_rad;
  do
    {
      MIGREP_Lk(tree);
      tree->mmod->rad -= 1.0;
    }
  while(tree->mmod->c_lnL > UNLIKELY + 0.1);

  min_rad = tree->mmod->rad + 2.0;
  tree->mmod->rad = ori_rad;
  MIGREP_Lk(tree);
  return(min_rad);
}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/
/* Get the minimum and maximum values a ldsk can take, given the position of the disk centre */
void MIGREP_Get_Min_Max_Ldsk_Given_Disk(t_ldsk *ldsk, phydbl **min, phydbl **max, t_tree *tree)
{
  phydbl *loc_min,*loc_max;
  int i;

  if(!ldsk->disk->next) return;

  loc_min = (phydbl *)mCalloc(tree->mmod->n_dim, sizeof(phydbl));
  loc_max = (phydbl *)mCalloc(tree->mmod->n_dim, sizeof(phydbl));

  For(i,tree->mmod->n_dim)
    {
      loc_min[i] = ldsk->disk->centr->lonlat[i] - tree->mmod->rad;
      loc_max[i] = ldsk->disk->centr->lonlat[i] + tree->mmod->rad;     
      
      if(ldsk->prev)
        {
          loc_min[i] = MAX(loc_min[i],ldsk->prev->disk->centr->lonlat[i] - tree->mmod->rad);
          loc_max[i] = MIN(loc_max[i],ldsk->prev->disk->centr->lonlat[i] + tree->mmod->rad);
        }

      loc_min[i] = MAX(0.0,loc_min[i]);
      loc_max[i] = MIN(tree->mmod->lim->lonlat[i],loc_max[i]);
    }

  (*min) = loc_min;
  (*max) = loc_max;
}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/
/* Get the minimum and maximum values a disk centre can take, given the position of the ldsk */
void MIGREP_Get_Min_Max_Disk_Given_Ldsk(t_dsk *disk, phydbl **min, phydbl **max, t_tree *tree)
{
  phydbl *loc_min,*loc_max;
  int i,j;
  phydbl tmp_min, tmp_max;

  if(!disk->next) return;

  loc_min = (phydbl *)mCalloc(tree->mmod->n_dim,sizeof(phydbl));
  loc_max = (phydbl *)mCalloc(tree->mmod->n_dim,sizeof(phydbl));

  if(!disk->ldsk || tree->mmod->name == MIGREP_NORMAL)
    {
      For(i,tree->mmod->n_dim)
        {
          loc_min[i] = 0.0;
          loc_max[i] = tree->mmod->lim->lonlat[i];
        }
    }
  else
    {
      For(i,tree->mmod->n_dim)
        {
          tmp_min = +INFINITY;
          tmp_max = -INFINITY;
          For(j,disk->ldsk->n_next)
            {
              if(disk->ldsk->next[j]->coord->lonlat[i] < tmp_min) tmp_min = disk->ldsk->next[j]->coord->lonlat[i];
              if(disk->ldsk->next[j]->coord->lonlat[i] > tmp_max) tmp_max = disk->ldsk->next[j]->coord->lonlat[i];
            }

          if(disk->ldsk->coord->lonlat[i] < tmp_min) tmp_min = disk->ldsk->coord->lonlat[i];
          if(disk->ldsk->coord->lonlat[i] > tmp_max) tmp_max = disk->ldsk->coord->lonlat[i];

          loc_min[i] = MAX(0.0,
                           tmp_max - tree->mmod->rad);

          loc_max[i] = MIN(tree->mmod->lim->lonlat[i],
                           tmp_min + tree->mmod->rad);
                    
          if(loc_max[i] < loc_min[i]) Generic_Exit(__FILE__,__LINE__,__FUNCTION__);
        }
    }

  (*min) = loc_min;
  (*max) = loc_max;
}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

void MIGREP_Update_Disk_Ldsk_Subtree(t_ldsk *root_ldsk, t_tree *tree)
{
  int i;  
  For(i,root_ldsk->n_next) MIGREP_Update_Disk_Ldsk_Subtree_Pre(root_ldsk,root_ldsk->next[i],root_ldsk,tree);
}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

void MIGREP_Update_Disk_Ldsk_Subtree_Pre(t_ldsk *old_ldsk, t_ldsk *young_ldsk, t_ldsk *root_ldsk, t_tree *tree)
{  
  if(!young_ldsk->disk->next) 
    {
      MIGREP_One_New_Traj_Given_Disk(young_ldsk,root_ldsk,tree);
      return;
    }
  else
    {
      int i;
      MIGREP_Update_Disk_Ldsk_Subtree_Pre(young_ldsk,young_ldsk->next[0],root_ldsk,tree);
      if(young_ldsk->n_next > 1) 
        {
          for(i=1;i<young_ldsk->n_next;i++) MIGREP_Update_Disk_Ldsk_Subtree_Pre(young_ldsk,young_ldsk->next[i],young_ldsk,tree);
        }
    }
}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

void MIGREP_Restore_Disk_Ldsk_Subtree(t_ldsk *root_ldsk, t_tree *tree)
{
  int i;  
  For(i,root_ldsk->n_next) MIGREP_Restore_Disk_Ldsk_Subtree_Pre(root_ldsk,root_ldsk->next[i],tree);
}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

void MIGREP_Restore_Disk_Ldsk_Subtree_Pre(t_ldsk *old_ldsk, t_ldsk *young_ldsk, t_tree *tree)
{  

  if(!young_ldsk->disk->next) return;
  else
    {
      int i;

      MIGREP_Restore_Geo_Coord(young_ldsk->coord);
      MIGREP_Restore_Geo_Coord(young_ldsk->disk->centr);

      For(i,young_ldsk->n_next)
        {
          MIGREP_Restore_Disk_Ldsk_Subtree_Pre(young_ldsk,young_ldsk->next[i],tree);
        }
    }
}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

void MIGREP_Proposal_Disk_Ldsk_Subtree(t_ldsk *root_ldsk, phydbl *logdens, t_tree *tree)
{
  int i;  
  (*logdens) = 0.0;
  For(i,root_ldsk->n_next) MIGREP_Proposal_Disk_Ldsk_Subtree_Pre(root_ldsk,root_ldsk->next[i],root_ldsk,logdens,tree);
}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

void MIGREP_Proposal_Disk_Ldsk_Subtree_Pre(t_ldsk *old_ldsk, t_ldsk *young_ldsk, t_ldsk *root_ldsk, phydbl *logdens, t_tree *tree)
{  

  if(!young_ldsk->disk->next) 
    {
      (*logdens) += MIGREP_Uniform_Path_Density(young_ldsk,root_ldsk,tree);  
      return;
    }
  else
    {
      int i;
      For(i,young_ldsk->n_next)
        {
          MIGREP_Proposal_Disk_Ldsk_Subtree_Pre(young_ldsk,young_ldsk->next[i],root_ldsk,logdens,tree);
        }
    }
}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/
/* Update the tree structure given the whole set of ldsk events */
/* Coalescent events involving multiple lineages are resolved using */
/* very short internal edges */
void MIGREP_Ldsk_To_Tree(t_tree *tree)
{
  int i,j;
  t_dsk *disk;

  /* Reset */
  For(i,2*tree->n_otu-1) 
    {
      For(j,3) 
        {
          tree->a_nodes[i]->v[j] = NULL;
          tree->a_nodes[i]->b[j] = NULL;
        }
    }

  disk = tree->disk->prev;
  while(disk) 
    {
      if(disk->ldsk) disk->ldsk->nd = NULL;
      disk = disk->prev;
    }

  /* Connect tips */
  For(i,tree->n_otu) 
    {
      tree->disk->ldsk_a[i]->nd = tree->a_nodes[i];
      tree->a_nodes[i]->coord = tree->disk->ldsk_a[i]->coord;
    }

  disk = tree->disk;
  while(disk->prev) disk = disk->prev;
  
  tree->n_root = tree->a_nodes[2*tree->n_otu-2];
  i = 2*tree->n_otu-3;
  tree->num_curr_branch_available = 0;
  MIGREP_Ldsk_To_Tree_Post(tree->n_root,disk->ldsk,&i,tree);

  For(i,tree->n_otu) 
    if(!tree->a_nodes[i]->v[0]) 
      Generic_Exit(__FILE__,__LINE__,__FUNCTION__);

  For(i,3) 
    if(tree->n_root->v[2]->v[i] == tree->n_root) 
      { 
        tree->n_root->v[2]->v[i] = tree->n_root->v[1]; 
        break; 
      }

  For(i,3) 
    if(tree->n_root->v[1]->v[i] == tree->n_root) 
      { 
        tree->n_root->v[1]->v[i] = tree->n_root->v[2]; 
        break; 
      }


  Connect_Edges_To_Nodes_Serial(tree);
  /* tree->num_curr_branch_available = 0; */
  /* Connect_Edges_To_Nodes_Recur(tree->a_nodes[0],tree->a_nodes[0]->v[0],tree); */

  tree->e_root = NULL;
  For(i,2*tree->n_otu-3)
    {
      /* printf("\n %d %d",tree->a_edges[i]->left->num,tree->a_edges[i]->rght->num); */
      if((tree->a_edges[i]->left == tree->n_root->v[1] && tree->a_edges[i]->rght == tree->n_root->v[2]) ||
         (tree->a_edges[i]->left == tree->n_root->v[2] && tree->a_edges[i]->rght == tree->n_root->v[1]))
        {        
          tree->e_root = tree->a_edges[i];
          break;
        }
    }
  if(tree->e_root == NULL) Generic_Exit(__FILE__,__LINE__,__FUNCTION__);
  
  /* For(i,2*tree->n_otu-1) */
  /*   { */
  /*     printf("\n. * Edge %d %p", */
  /*            tree->a_edges[i]->num, */
  /*            tree->a_edges[i]); */
  /*   } */
}
/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

void MIGREP_Ldsk_To_Tree_Post(t_node *a, t_ldsk *ldsk, int *available, t_tree *tree)
{
  if(ldsk == NULL) Generic_Exit(__FILE__,__LINE__,__FUNCTION__);
  if(a == NULL)    Generic_Exit(__FILE__,__LINE__,__FUNCTION__);

  ldsk->nd = a;  
  tree->rates->nd_t[a->num] = ldsk->disk->time;
  a->coord = ldsk->coord;

  if(!ldsk->next) return;
  else
    {
      t_node *parent,*son;
      int n_next;
      t_ldsk *t;

      parent       = a;
      parent->v[1] = NULL;
      parent->v[2] = NULL;
      n_next       = 0;
      do
        {
          t = ldsk->next[n_next]; 
          while(t->next && t->n_next == 1) t = t->next[0];
         
          if(t->nd == NULL) 
            {
              son = tree->a_nodes[*available];
              (*available) = (*available)-1;
            }
          else
            {
              son = t->nd;
            }
                    
          MIGREP_Ldsk_To_Tree_Post(son,t,available,tree);          

          if(parent->v[2] != NULL && n_next >= 2) 
            {
              t_node *new_parent;
              /* phydbl orig_l2; */

              new_parent = tree->a_nodes[*available];
              (*available) = (*available)-1;              

              new_parent->v[0]       = parent;
              new_parent->v[1]       = parent->v[2];
              new_parent->v[2]       = son;

              parent->v[2]           = new_parent;
              son->v[0]              = new_parent;
              new_parent->v[1]->v[0] = new_parent;
              
              /* printf("\n# connect %d to %d",parent->num,new_parent->num); */
              /* printf("\n# connect %d to %d",new_parent->num,new_parent->v[1]->num); */
              /* printf("\n# connect %d to %d",new_parent->num,new_parent->v[2]->num); */              
              /* fflush(NULL); */
              
              tree->rates->nd_t[new_parent->num] = ldsk->disk->time;

              parent = new_parent;
            }
          else
            {
              son->v[0] = parent;          
              if(!parent->v[1]) parent->v[1] = son;
              else              parent->v[2] = son;
              
              /* printf("\n. connect %d to %d",parent->num,son->num); */
            }

          n_next++;
        }
      while(n_next != ldsk->n_next);
    }
}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

void MIGREP_Remove_Lindisk_Next(t_ldsk *ldsk, t_ldsk *rm)
{
  t_ldsk **new_next;
  int i,pos;

  new_next = (t_ldsk **)mCalloc(ldsk->n_next-1+NEXT_BLOCK_SIZE,sizeof(t_ldsk *));

  pos = 0;
  For(i,ldsk->n_next)
    {
      if(ldsk->next[i] != rm)
        {
          new_next[pos] = ldsk->next[i];
          pos++;
        }
    }

  ldsk->n_next--;
  Free(ldsk->next);
  ldsk->next = new_next;
  /* printf("\n. remove next for ldsk %s n_next set to %d",ldsk->coord->id,ldsk->n_next); */
  /* fflush(NULL); */
}  

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/
/* Returns the vector of average pairwise distances between ldsk on each disk */
phydbl *MIGREP_Mean_Pairwise_Distance_Between_Lineage_Locations(t_tree *tree)
{
  phydbl *dist;
  int block,n_disks,i,j, k;
  t_dsk *disk;
  
  MIGREP_Update_Lindisk_List(tree);

  dist = NULL;
  block   = 100;
  disk    = tree->disk;
  n_disks = 0;
  do
    {
      if(!n_disks) dist = (phydbl *)mCalloc(block,sizeof(phydbl));
      else if(!(n_disks%block)) dist = (phydbl *)mRealloc(dist,n_disks+block,sizeof(phydbl));
      
      dist[n_disks] = 0.0;
      For(i,disk->n_ldsk_a-1)
        {
          for(j=i+1;j<disk->n_ldsk_a;j++)
            {
              For(k,tree->mmod->n_dim)
                {
                  dist[n_disks] += FABS(disk->ldsk_a[i]->coord->lonlat[k] - disk->ldsk_a[j]->coord->lonlat[k]);
                  printf("\n * %d %f %f %f",
                         k,
                         disk->ldsk_a[i]->coord->lonlat[k],
                         disk->ldsk_a[j]->coord->lonlat[k],
                         tree->mmod->lim->lonlat[k]);
                }
            }
        }
      dist[n_disks] /= (phydbl)(disk->n_ldsk_a * (disk->n_ldsk_a-1) / 2.);

      printf("\n %d %f %f",disk->n_ldsk_a,disk->time,dist[n_disks]);

      n_disks++;
      disk = disk->prev;
    }
  while(disk->prev);

  return(dist);

}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

phydbl MIGREP_Random_Select_Time_Between_Jumps(t_tree *tree)
{
  t_dsk  *disk,**valid_disks;
  int n_valid_disks,block,select;
  phydbl time;

  valid_disks   = NULL;
  disk          = NULL;
  block         = 100;

  if(tree->disk->next) Generic_Exit(__FILE__,__LINE__,__FUNCTION__);
  disk = tree->disk->prev;
  n_valid_disks = 0;
  do
    {
      if(disk->ldsk != NULL && disk->prev != NULL)
        {
          if(!n_valid_disks) valid_disks = (t_dsk **)mCalloc(block,sizeof(t_dsk *));
          else if(!(n_valid_disks%block)) valid_disks = (t_dsk **)mRealloc(valid_disks,n_valid_disks+block,sizeof(t_dsk *));
          valid_disks[n_valid_disks] = disk;
          n_valid_disks++;
        }
      disk = disk->prev;
    }
  while(disk->prev);

  if(!n_valid_disks) return -1.0;

  select = Rand_Int(0,n_valid_disks-1);

  time = valid_disks[select]->time - valid_disks[select]->ldsk->prev->disk->time;
  
  Free(valid_disks);

  return(time);
}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

int MIGREP_Is_In_Ldscape(t_ldsk *ldsk, t_migrep_mod *mmod)
{
  int j;
  For(j,mmod->n_dim) if(ldsk->coord->lonlat[j] > mmod->lim->lonlat[j]) return NO;
  return YES;
}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

phydbl MIGREP_Mean_Time_Between_Events(t_tree *tree)
{
  int n_inter;
  phydbl T;

  n_inter = MIGREP_Total_Number_Of_Intervals(tree);  
  T = -tree->rates->nd_t[tree->n_root->num];
  return((phydbl)(T/n_inter));
}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

/* Uses the regression technique described in Barton et al. TPB (2013)
   to estimate the size of the neighborhood when the population evolve
   according to a spatial Lambda-Fleming-Viot process.
*/
phydbl MIGREP_Neigborhood_Size_Regression(t_tree *tree)
{
  int i,j,pair;
  t_node *anc;
  phydbl *dist,min_dist;
  phydbl QA,Qr,*fst,fst0,fst_min_dist;
  phydbl cov_fst_dist,var_dist,slope;
  phydbl eps;

  eps = 1.E-10;

  QA = Mean_Identity(tree->data);

  fst  = (phydbl *)mCalloc(tree->n_otu*(tree->n_otu-1)/2,sizeof(phydbl));
  dist = (phydbl *)mCalloc(tree->n_otu*(tree->n_otu-1)/2,sizeof(phydbl));

  pair = 0;
  fst0 = 0.0;
  For(i,tree->n_otu-1)
    {
      fst_min_dist = 0.0;
      min_dist = MDBL_MAX;
      for(j=i+1;j<tree->n_otu;j++)
        {
          anc = Find_Lca_Pair_Of_Nodes(tree->a_nodes[i],tree->a_nodes[j],tree);
          if(anc == NULL) 
            {
              PhyML_Printf("\n. %s",Write_Tree(tree,NO));
              PhyML_Printf("\n. %s %s",tree->a_nodes[i]->name,tree->a_nodes[j]->name);
              Generic_Exit(__FILE__,__LINE__,__FUNCTION__);            
            }
          
          dist[pair] = Euclidean_Dist(tree->a_nodes[i]->coord,tree->a_nodes[j]->coord);
          dist[pair] = LOG(dist[pair]);

          Qr = Pairwise_Identity(i,j,tree->data);
          fst[pair] = (Qr-QA)/(1.-QA);

          if(dist[pair] < min_dist)
            {
              min_dist     = dist[pair];
              fst_min_dist = fst[pair];
            }

          pair++;
        }
      fst0 += fst_min_dist;
    }

  fst0 /= (phydbl)(tree->n_otu);

  cov_fst_dist = Covariance(dist,fst,pair);
  var_dist = Variance(dist,pair);

  slope = cov_fst_dist / var_dist;

  Free(dist);
  Free(fst);

  return((fst0-1.+eps)/(slope+eps));
}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

void MIGREP_Rand_Pairs_Coal_Times_Dist(t_tree *tree)
{
  t_node *anc;
  phydbl dist;
  int i, j;

  i = Rand_Int(0,tree->n_otu-1);
  j = Rand_Int(0,tree->n_otu-1);

  if(i == j) PhyML_Printf("\nxxWxx 0.0 0.0");
  else
    {

      anc = Find_Lca_Pair_Of_Nodes(tree->a_nodes[i],tree->a_nodes[j],tree);
      if(anc == NULL) 
        {
          PhyML_Printf("\n. %s",Write_Tree(tree,NO));
          PhyML_Printf("\n. %s %s",tree->a_nodes[i]->name,tree->a_nodes[j]->name);
          Generic_Exit(__FILE__,__LINE__,__FUNCTION__);            
        }
      
      PhyML_Printf("\nxxWxx %12f",tree->rates->nd_t[anc->num]);
      dist = Euclidean_Dist(tree->a_nodes[i]->coord,tree->a_nodes[j]->coord);
      PhyML_Printf(" %f",dist);
    }
}


/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

phydbl MIGREP_Neigborhood_Size(t_tree *tree)
{
  switch(tree->mmod->name)
    {
    case MIGREP_UNIFORM: { return(1./tree->mmod->mu); break; }
    case MIGREP_NORMAL:  { return(2./tree->mmod->mu); break; }
    }
  return(-1.);
}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/

phydbl MIGREP_Dist_Parent_To_Offspring(t_tree *tree)
{  
  switch(tree->mmod->name)
    {
    case MIGREP_UNIFORM: { return(-1.0); break;}
    case MIGREP_NORMAL:  
      { 
        return(4.*PI*
               tree->mmod->lbda/(tree->mmod->lim->lonlat[0]*tree->mmod->lim->lonlat[1])*
               POW(tree->mmod->rad,4)*
               tree->mmod->mu); 
        break; 
      }
    }
  return(-1.);
}

/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/
/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/
/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/
/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/
/*////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////*/
