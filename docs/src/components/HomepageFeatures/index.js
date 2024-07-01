import clsx from 'clsx';
import Heading from '@theme/Heading';
import styles from './styles.module.css';

const FeatureList = [
  {
    title: 'Easy to Use',
    description: (
      <>
        No complex set up - optimization algorithms can be run in as little as one line of code.
      </>
    ),
  },
  {
    title: 'Highly Customizable',
    description: (
      <>
        Easily customize optimization hyperparameters and define your own parameter functions to fit your specific needs.
      </>
    ),
  },
  {
    title: 'Versatile',
    description: (
      <>
        Wide variety of algorithms allows for optimization of diverse functions, from simple to complex.
      </>
    ),
  },
];

function Feature({title, description}) {
  return (
    <div className={clsx('col col--4', styles.feature)}>
      <div className="text--center padding-horiz--md">
        <Heading as="h3">{title}</Heading>
        <p>{description}</p>
      </div>
    </div>
  );
}

export default function HomepageFeatures() {
  return (
    <section className={styles.features}>
      <div className="container">
        <div className="row">
          {FeatureList.map((props, idx) => (
            <Feature key={idx} {...props} />
          ))}
        </div>
      </div>
    </section>
  );
}
